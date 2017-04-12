#pragma once

#include "BulletCmp.hpp"
#include "BodyCmp.hpp"
#include "PlayerSystem.hpp"
#include "CollisionComponent.hpp"
#include <entities/System.hpp>
#include <common/GameMessages.hpp>
#include <game/World.hpp>
#include <game/CollisionManager.hpp>
#include <game/GameEngine.hpp>
#include <networking/ServerNetworkManager.hpp>
#include <voyage.pb.h>

class ServerPlayerSystem : public PlayerSystem {

public:

	ServerPlayerSystem(World::Ptr world,
			   ServerNetworkManager & net) : PlayerSystem(world),
							 m_network_manager(net) {
		// search for entity and update network component
	}

	void configure(EventManager &events) override {
		events.subscribe<UserAction>(*this);
	}

	// Receives a user action from the client
	// In order to know from which player/entity this message comes we should wrap it back into
	// the UserAction structure from the network handler!
	void receive(const UserAction &user_action) {

		assert(not GameEngine::isClient);

		//add client messages to the queue
		m_client_msg_queue[user_action.player_id].push(user_action);
	}

	// Computes player state as an interpolation of the two previous states
	BodyState interpolateToNewState(const PlayerCmp & body) {

	}

	bool messagesWaiting(uint32_t player_id) {
		return m_client_msg_queue[player_id].size() > 0;
	}

	UserAction getFirstMessage(uint32_t player_id) {
		return m_client_msg_queue[player_id].front();
	}

	void popFirstMessage(uint32_t player_id) {
		m_client_msg_queue[player_id].pop();
	}

	void update(EntityManager & em, EventManager &evm, float delta ) {

		em.each<NetworkCmp, PlayerCmp, BodyCmp>
			([&em, delta, this](Entity entity,
					    NetworkCmp &netdata,
					    PlayerCmp &player,
					    BodyCmp & body) {

				// get any client messages in the queue for this player/entity
				Entity::Id player_id = player.player_id;

				if (messagesWaiting(player_id.index()))  {
					UserAction action =
						getFirstMessage(player_id.index());

					// TODO: compute delta as the time since
					// the last message - RTT/2

					// move entity (this is replicated in client: REFACTOR)
					Vec3f tmp_move_vec = action.motion_vec *
						body.m_moveSpeed * action.delta;

					Vec3f correction_vector =
					CollisionManager::computeCorrectionVector(m_world,
										  body.m_position,
										  body.getPolygon(),
										  tmp_move_vec);
					// Update body
					body.m_moveVec = tmp_move_vec - correction_vector;
					body.m_rotAngle = action.angle + M_PI/4;
					body.m_position += body.m_moveVec;

					// set visibility manager with player position
					//m_visibilityManager.setPlayerPosition(body.m_position);

					// push the last action_sequence received
					//m_network_manager->pushActionSeq(player.player_id, action.seq_num);
					// compute player state with new data
					// NOTE: the actual position will be updated later in
					// BodySystem. A bit ugly

					voyage::sc_playerState ps;
					ps.set_actionseq(action.seq_num);
					ps.set_pos_x(body.m_position(0));
					ps.set_pos_y(body.m_position(1));
					ps.set_angle(body.m_rotAngle);

					netdata.dirty = true;

					m_network_manager.pushPlayerStateMsg(player.player_id.index(), ps);

					popFirstMessage(player_id.index());
				}




		   });
	}

private:

	//VisibilityManager & m_visibilityManager;
	ServerNetworkManager & m_network_manager;
	std::map<uint32_t, std::queue<UserAction> > m_client_msg_queue;

};

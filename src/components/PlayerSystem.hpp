#pragma once

#include <entities/System.hpp>
#include <game/World.hpp>
#include <game/CollisionManager.hpp>
#include "GraphicsCmp.hpp"
#include "InputCmp.hpp"
#include "BulletCmp.hpp"
#include "BodyCmp.hpp"
#include "CollisionComponent.hpp"
#include <pumpkin.hpp>
#include <voyage.pb.h>


class UserAction {

public:
	UserAction(avec mv) : move_vector(mv) {}
	avec move_vector;
	float angle = 0;
};

class PlayerCmp : public BodyCmp{

public:
	PlayerCmp() {}

	std::string player_name;
	//player session?
	int player_id;
};


class PlayerSystem : public System<PlayerSystem>,
		     public Receiver<voyage::cs_userAction>
		     public Receiver<voyage::sc_playerState>{
public:

	EntityNetworkSystem(World::Ptr world,
			    ClientNetworkManager::Ptr net) : m_world(world),
							     m_network_manager(net) {

		// search for entity and update network component
		
	}

	void sendActionToServer(const UserAction & action) {

		voyage::cs_userAction actionMsg;
		actionMsg.set_seq_num(action.seq_num);
		actionMsg.set_motion_x(action.motion_vec(0));
		actionMsg.set_motion_y(action.motion_vec(1));
		actionMsg.set_angle(action.angle);
		
		m_network_manager->sendData<voyage::cs_userAction>(ID_CS_USER_ACTION, actionMsg);
	}

	// Receives a user action from the client
	void receive(const voyage::cs_userAction &user_action) {


	}
	
	// Receives an entity update message from server
	void receive(const voyage::sc_playerState &entity_update) {

		std::cout << "Received entity update in network manager!!" << std::endl;
		std::cout << "pos:" << entity_update.x() << ","
			  << entity_update.y() << std::endl;

		// keep a map of received states from server
		m_message_map[entity_update.entityid()] = entity_update;
		
	}

	// Computes the player state with the last state from the server and the queue of
	// UserActions
	BodyState computeConsistentState(const BodyState & server_state) {		
		
	}


	BodyState interpolateToNewState(const BodyState & body) {

	}

	
	voyage::sc_playerState popPlayerMsg() {
		voyage::sc_playerState msg = m_player_state[entity.id];
		m_player_state.erase(entity.id);
		return msg;
	}
	
	void update(EntityManager & em, EventManager &evm, float delta ) {

		GameMap::Ptr game_map = m_world->getGameMap();

		em.each<NetworkCmp, BodyCmp>
			([&em, delta, this](Entity entity,
					  NetworkCmp &netdata,
					  PlayerCmp &player) {

			if (isClient) {				
				
				// Update with input data
				if (entity.id == m_playerId) {

					if (PlayerInput::inputIsActive()) {
						
						UserAction action = PlayerInput::getUserAction();
						action.setSeqId(action_seq++);

						m_action_queue.push(action);
					
						sendActionToServer(action);
						avec tmp_move_vec = move_vector + body.m_moveSpeed * delta;

						move_vector = tmp_move_vec - correction_vector;

					}

					if (m_player_state.count(entity.id)) {

						voyage::sc_playerState server_body =
							popPlayerMsg(entity.id);

						// using server last state + queue of user actions,
						// compute new body state.
						BodyState consistent_state =
							computConsistentState(server_body);

						body.update(consistent_state);
					}

				}
				else {

					if (m_player_state.count(entity.id)) {

						voyage::sc_playerState server_body =
							popPlayerMsg(entity.id);
						

						// the body keeps a queue of the past
						// two states to interpolate
						body.push_new_state(server_body);
						
					}
					
					BodyState interpolated_body = 
						interpolateToNewState(body);
					
					body.update(interpolated_body);
					
				}
			}
			else {

				// get any client messages in the queue for this entity
				
				

				// correct motionVector for map collisions
				UserAction correctedAction =
					CollisionManager::computeCorrectionVector();

				// update World state
				body.update(action);
				
			}

		   });
	}

private:

	World::Ptr m_world;
	ClientNetworkManager::Ptr m_network_manager;
	std::map<uint32_t, voyage::sc_playerState> m_message_map;

	
	static uint32_t action_seq = 0;
	std::queue<UserAction> m_action_queue;
};

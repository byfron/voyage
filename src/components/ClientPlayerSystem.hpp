#pragma once

#include "BulletCmp.hpp"
#include "BodyCmp.hpp"
#include "PlayerSystem.hpp"
#include "CollisionComponent.hpp"
#include "InventoryComponent.hpp"
#include "WeaponSystem.hpp"
#include <entities/System.hpp>
#include <common/GameMessages.hpp>
#include <game/World.hpp>
#include <game/CollisionManager.hpp>
#include <game/GameEngine.hpp>
#include <networking/ClientNetworkManager.hpp>
#include <utils/PlayerInput.hpp>
#include <visibility/VisibilityManager.hpp>
#include <voyage.pb.h>


static unsigned long getTimeSinceEpochMillis()
{
    return static_cast<unsigned long>
        (std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count());
}


using namespace PlayerInput;

class ClientPlayerSystem : public PlayerSystem {

public:

	ClientPlayerSystem(World::Ptr world,
			   NetworkMessagePool & msgPool,
			   ClientNetworkManager & net,
                       VisibilityManager & vm): PlayerSystem(world),
                                                m_msgPool(msgPool),
                                                m_network_manager(net),
                                                m_visibilityManager(vm) {

		// search for entity and update network component
		m_action_queue.push(UserAction());

	}

	void sendActionToServer(const UserAction & action) {

		assert(GameEngine::isClient);

		voyage::cs_userAction actionMsg;
		actionMsg.set_seq_num(action.seq_num);
		actionMsg.set_motion_x(action.motion_vec(0));
		actionMsg.set_motion_y(action.motion_vec(1));
		actionMsg.set_angle(action.angle);
		actionMsg.set_delta(action.delta);
		actionMsg.set_timestamp(getTimeSinceEpochMillis());

		m_network_manager.sendData<voyage::cs_userAction>(ID_CS_USER_ACTION, actionMsg);
	}

	// Computes the player state with the last state from the server and the queue of
	// UserActions
	BodyState computeConsistentState(const voyage::sc_playerState & server_state) {

	}


	// void movePlayer() {

	// }

	// void shoot() {

	// }


	void update(EntityManager & em, EventManager &evm, float delta ) {

		em.each<NetworkCmp, PlayerCmp, BodyCmp>
			([&em, delta, this](Entity entity,
					    NetworkCmp &netdata,
					    PlayerCmp &player,
					    BodyCmp & body) {


			// Update with input data
			if (entity.id() == GameEngine::m_playerId)
			{

				UserAction action = PlayerInput::getUserAction();

				// send the delta with the user action now
				action.delta = delta;

				UserAction last_action = m_action_queue.front();
				m_action_queue.pop();



				// TODO: Only when the action changes?
				// we register it and send ot to the server
				//if (action != last_action)
				if ((action.motion_vec.norm() != 0) ||
					action.angle != last_action.angle ||
					action.action_code != last_action.action_code)
				{
					spd::get("Client")->info("sending input update");

					action.seq_num = m_action_seq_num++;


					// TODO DO NOT TRANSFORM DE ACTION WITH
					// THE CAMERA ROTATION!
					sendActionToServer(action);

				}

				m_action_queue.push(action);


				////////////////////////////////////////////////////////////
				// TODO: refactor this (repeated in ServerPlayerSystem)
				Vec3f tmp_move_vec = action.motion_vec *
					body.m_moveSpeed * delta;

				Vec3f correction_vector =
					CollisionManager::computeCorrectionVector(m_world,
										  body.m_position,
										  body.getPolygon(),
										  tmp_move_vec);

				// applyAction
				if (action.action_code & (1 << int(Action::SHOOTING))) {
					InventoryComponent* inv = em.getComponentPtr<InventoryComponent>(entity.id());
					WeaponComponent* weapon = em.getComponentPtr<WeaponComponent>(inv->getActiveWeaponId());
					Vec2f from, direction;
					weapon->fire(delta, em, from, direction);
					std::cout << "firing weapon" << std::endl;
				}

				// Update body motion
				body.m_moveVec = tmp_move_vec - correction_vector;
				body.m_rotAngle = action.angle
					+ M_PI/4;
				body.m_position += body.m_moveVec;
				////////////////////////////////////////////////////////////

				body.updateRotation();

				// Move camera
				pumpkin::GraphicsEngine::camera().moveAlong(body.m_moveVec);

				// Update visibilityManager with new player position
				m_visibilityManager.update(body.m_position.segment<2>(0, 2), m_world);

				// HERE. check for messages of this entity in the common
				// WorldUpdate pool
				if (m_msgPool.hasPlayerMsg()) {

					voyage::sc_playerState server_body =
						m_msgPool.popPlayerUpdateMsg();

					// using server last state + queue of user actions,
					// compute new body state.
					BodyState consistent_state =
						computeConsistentState(server_body);

					// TODO
					//player.update(consistent_state);
				}
			}
			else { // other players

				if (m_msgPool.hasEntityMsg(entity.id().index())) {

					voyage::sc_entityState update =
						m_msgPool.popEntityUpdateMsg(entity.id().index());
					body.m_position(0) = update.pos_x();
					body.m_position(1) = update.pos_y();
					body.m_rotAngle = update.angle();
					body.updateRotation();

				}

			}
		});
	}

private:

	VisibilityManager & m_visibilityManager;
	ClientNetworkManager & m_network_manager;
	NetworkMessagePool & m_msgPool;
	uint32_t m_action_seq_num;
	std::queue<UserAction> m_action_queue;
};

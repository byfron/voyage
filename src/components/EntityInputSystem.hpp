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

struct Quad {
	Vec2f vertex[4];
	int num_vertices = 4;
};

/* 
UPDATE:
- Get input, send to server, and perform prediction. 
- Check if server data available.
- Sync current pos with server data
*/

class EntityInputSystem : public System<EntityInputSystem>, public Receiver<voyage::sc_entityUpdate> {
public:

	EntityInputSystem(World::Ptr world,
			  ClientNetworkManager::Ptr net) : m_world(world),
							   m_network_manager(net) {

	}

	
	// Receives an entity update message from server
	void receive(const voyage::sc_entityUpdate &entity_update) {

		std::cout << "Received entity update!" << std::endl;//entity_update.entityId() << std::endl;
		std::cout << "pos:" << entity_update.x() << "," << entity_update.y() << std::endl;

		// keep a map of received states from server
		
	}
	
	void update(EntityManager & em, EventManager &evm, float delta ) {

		GameMap::Ptr game_map = m_world->getGameMap();

		// Player entity in server will not have a InputComponent
		// and this will never be run
		em.each<InputCmp, CollisionComponent, BodyCmp>
			([&em, delta, this](Entity entity,
					  InputCmp &input,
					  CollisionComponent &col,
					  BodyCmp &body) {


			// make this bodycmp
			// with if(client)

				
			// Updates inputs
			input.update();

			// Add client input to a action queue
			// TODO
			
			 
			// Inmediatelly send movement vector
			voyage::cs_userMovement movement;
			movement.set_entityid(entity.id().id);
			movement.set_x(input.m_move_vector(0));
			movement.set_y(input.m_move_vector(1));				
			this->m_network_manager->sendData<voyage::cs_userMovement>(ID_CS_USER_MOVEMENT, movement);


			// TODO: the polygon should come from the collision?
			// Solve collisions
			Vec3f tmp_move_vec = input.m_move_vector *
				body.m_moveSpeed * delta;
			Vec3f correction_vector =
				CollisionManager::computeCorrectionVector(this->m_world,
									  body.m_position,
									  body.getPolygon(),
									  tmp_move_vec);
								       
			// Move player
			body.m_moveVec = tmp_move_vec - correction_vector;

			// Move camera
			pumpkin::GraphicsEngine::camera().moveAlong(body.m_moveVec);

			body.m_rotAngle = pumpkin::InputManager::m_mouse_angle
				+ M_PI/4;			

		   });
	}

private:

	World::Ptr m_world;
	ClientNetworkManager::Ptr m_network_manager;
};

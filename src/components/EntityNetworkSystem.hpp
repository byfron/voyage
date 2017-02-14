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


class NetworkCmp {

public:
	NetworkCmp() {}	
};


class PlayerSystem : public System<PlayerSystem>,
		     public Receiver<voyage::sc_entityUpdate> {
public:

	EntityNetworkSystem(World::Ptr world,
			    ClientNetworkManager::Ptr net) : m_world(world),
							     m_network_manager(net) {

		// search for entity and update network component
		
	}
	
	// Receives an entity update message from server
	void receive(const voyage::sc_entityUpdate &entity_update) {

		std::cout << "Received entity update in network manager!!" << std::endl;
		std::cout << "pos:" << entity_update.x() << ","
			  << entity_update.y() << std::endl;

		// keep a map of received states from server
		m_message_map[entity_update.entityid()] = entity_update;
		
	}	
	
	void update(EntityManager & em, EventManager &evm, float delta ) {

		GameMap::Ptr game_map = m_world->getGameMap();

		em.each<NetworkCmp, BodyCmp>
			([&em, delta, this](Entity entity,
					  NetworkCmp &netdata,
					  BodyCmp &body) {


			if (isClient) {

				
				
				// Update entities with server messages (client pred)
				
			}
			else {
				
				// Update entities with client messages

			}



				

			// go thorugh network components
			if (m_message_map.count(entity.id().id)) {
				voyage::sc_entityUpdate msg = m_message_map[entity.id().id];
				body.m_auth_position = Vec3f(msg.x(), msg.y(), 0.0);

				// delete entry
				m_message_map.erase(entity.id().id);
			}		
		   });
	}

private:

	World::Ptr m_world;
	ClientNetworkManager::Ptr m_network_manager;
	std::map<uint32_t, voyage::sc_entityUpdate> m_message_map;
};

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


class PlayerSystem : public System<PlayerSystem>,
		     public Receiver<voyage::sc_entityUpdate> {
public:

	EntityNetworkSystem(World::Ptr world,
			    ClientNetworkManager::Ptr net) : m_world(world),
							     m_network_manager(net) {

		// search for entity and update network component
		
	}
	
	// Receives an entity update message from server
	void receive(const voyage::sc_entityUpdate &entity_update);
	
	void update(EntityManager & em, EventManager &evm, float delta);
private:

	World::Ptr m_world;
	ClientNetworkManager::Ptr m_network_manager;
	std::map<uint32_t, voyage::sc_entityUpdate> m_message_map;
};

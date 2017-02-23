#pragma once

#include <components/BodyCmp.hpp>
#include <networking/Messages.hpp>

using namespace voyage;

class MessageFactory {

public:

	static Message<sc_entitySpawn>::Ptr createSpawnPlayerMsg(uint32_t id, const BodyCmp &body) {
		sc_entitySpawn spawn_player;
		spawn_player.set_entityid(id);
		spawn_player.set_x(body.m_position(0));
		spawn_player.set_y(body.m_position(1));
		return std::make_shared<Message<sc_entitySpawn> >(ID_SC_SPAWN_PLAYER, spawn_player);
	}

	static Message<sc_entitySpawn>::Ptr createSpawnEntityMsg(uint32_t id, const BodyCmp &body) {
		sc_entitySpawn spawn_entity;
		spawn_entity.set_entityid(id);
		spawn_entity.set_x(body.m_position(0));
		spawn_entity.set_y(body.m_position(1));
		return std::make_shared<Message<sc_entitySpawn> >(ID_SC_SPAWN_ENTITY, spawn_entity);
	}
};

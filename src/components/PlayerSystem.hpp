#pragma once

#include "BulletCmp.hpp"
#include "CollisionComponent.hpp"
#include <entities/System.hpp>
#include <common/GameMessages.hpp>
#include <game/World.hpp>
#include <game/CollisionManager.hpp>
#include <game/GameEngine.hpp>
#include <networking/ClientNetworkManager.hpp>
#include <voyage.pb.h>

struct UserAction {

	UserAction() {}
	UserAction(const int & pid, const voyage::cs_userAction & ua) : player_id(pid) {

		seq_num = ua.seq_num();

//		TODO: if (ua.has blabla)
		motion_vec = Vec3f::Zero();
		motion_vec(0) = ua.motion_x();
		motion_vec(1) = ua.motion_y();
		angle = ua.angle();
		action_code = ua.action_code();
		delta = ua.delta();
	}

	int player_id = -1;
	Vec3f motion_vec;
	float angle = 0;
	uint32_t action_code = 0;
	uint32_t seq_num;
	float delta;

	bool operator !=(const UserAction &a) {
		return (a.motion_vec != motion_vec ||
			a.angle != angle ||
			a.action_code != action_code);
	}
};

class NetworkCmp {

public:
	bool dirty = true;
};

class PlayerCmp  {

public:
	PlayerCmp(Entity::Id pid) : player_id(pid) {}

	std::string player_name;
	Entity::Id player_id;

};

class PlayerSystem : public System<PlayerSystem>,
		     public Receiver<PlayerSystem> {
public:

	PlayerSystem(World::Ptr world) : m_world(world) {}
	void update(EntityManager & em, EventManager &evm, float delta );

protected:

	World::Ptr m_world;
};

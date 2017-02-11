#pragma once
#include <entities/System.hpp>

class BulletCmp {

public:

	int m_type;
	float m_damage;
	float m_lifespan;
	
};


class BulletSystem : public System<BulletSystem>, public Receiver<BulletSystem> {


	// public:

	// void configure(EventManager &events) {
	// 	events.subscribe<CollisionEvent>(this);
	// }

	// void receive(const CollisionEvent &collision) {
	// 	m_collided_bullets.insert(collision.left);

	// 	// emit particles on impact
		
	// 	// send damage message to entity
		
	// }

	// void update(float delta) {

	// 	// destroy collided bullets during the update
	// 	for (auto e : m_collided_bullets) {
	// 		e.destroy();
	// 	}
		
	// }

	// private:

	// std::unordered_set<Entity> m_collided_bullets;
	
};

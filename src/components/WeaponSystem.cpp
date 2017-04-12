#include "BodyCmp.hpp"
#include "GraphicsCmp.hpp"
#include "PlayerSystem.hpp"
#include "WeaponSystem.hpp"


void WeaponComponent::fire(float delta, EntityManager & em,
						   const Vec2f & from, const Vec2f & direction) {

	if (is_firing) {
		if (bulletInChamber(delta)) {
			time_since_last_shot = 0.0f;
			generateBullets(em, from, direction);
		}

		// switch(type) {
		// case WeaponType::GUN_TYPE: //TODO: separate in two different components
		// 	shoot(delta);
		// case WeaponType::KNIFE_TYPE:
		// 	stab(delta);
		// };
	}
}

void WeaponComponent::generateBullets(EntityManager & em, const Vec2f & from,
									  const Vec2f & direction) {

	// how many bullets?
	Entity bullet = em.createNetworked();
	em.assign<BodyCmp>(bullet.id(), bullet_speed, TO_3DVEC(direction), TO_3DVEC(from));
	em.assign<NetworkCmp>(bullet.id());
	em.assign<CollisionComponent>(bullet.id());

	//assign graphics if is the client!
	if (GameEngine::isClient) {
		//	em.assign<GraphicsCmp>(bullet.id(), ConfigManager::getBulletConfig());
	}
}

bool WeaponComponent::bulletInChamber(float delta) {
	time_since_last_shot+=delta;
	return time_since_last_shot >= rate_of_fire;
}


// // TODO can't we run fire directly from the clientsystem???

// void WeaponSystem::update(EntityManager & em, EventManager &evm, float delta ) {

// 	em.each<WeaponComponent>([&em, delta](Entity entity,
// 										  WeaponComponent & weapon) {

// 								 weapon.fire(delta);
// 	});

// }

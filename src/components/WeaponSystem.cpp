#include "BodyComponent.hpp"
#include "ParticleComponent.hpp"
#include "PlayerComponent.hpp"
#include "WeaponSystem.hpp"

#if CLIENT
#include "GraphicsComponent.hpp"
#endif

void WeaponComponent::fire(float delta, EntityManager & em,
						   const Vec3f & from, const Vec3f & direction) {
	if (is_firing) {
		if (bulletInChamber(delta)) {
			time_since_last_shot = 0.0f;
			generateBullets(em, from, direction);
		}

		// switch(type) {
		// case WeaponType::GUN_TYPE: //TODO: separate in two different components
		// shoot(delta);
		// case WeaponType::KNIFE_TYPE:
		// stab(delta);
		// };
	}
}

void WeaponComponent::generateBullets(EntityManager & em, const Vec3f & from,
									  const Vec3f & direction) {
	// how many bullets
	Entity bullet = em.createLocal();
	em.assign<BodyComponent>(bullet.id(), bullet_speed, direction, from);
	em.assign<NetworkComponent>(bullet.id());
	em.assign<CollisionComponent>(bullet.id());
	uint16_t particle_type = 0;
	em.assign<ParticleComponent>(bullet.id(), particle_type);

	//assign graphics if is the client!
#if CLIENT
		em.assign<ParticleGraphicsComponent>(bullet.id(), particle_type);
#endif

}

bool WeaponComponent::bulletInChamber(float delta) {
	return true;
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

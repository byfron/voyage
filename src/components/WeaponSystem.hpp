#pragma once

#include <entities/System.hpp>
#include <common/math.hpp>

enum WeaponType {

};

class WeaponComponent {

public:

	void fire(float delta, EntityManager & em, const Vec3f & from, const Vec3f & direction);
	bool bulletInChamber(float delta);
	void generateBullets(EntityManager & em, const Vec3f & from,
						 const Vec3f & direction);

	bool is_firing = true;
	float bullet_speed = 10.0;
	WeaponType type;
	int num_bullets;
	float damage;

	float time_since_last_shot = 0;
	float rate_of_fire = 500; //milliseconds
};

class WeaponSystem : public System<WeaponSystem> {

public:

	void update(EntityManager & em, EventManager &evm, float delta ) {


	}




};

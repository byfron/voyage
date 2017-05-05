#pragma once

#include <entities/System.hpp>

class ParticleSystem : public System<ParticleSystem>{

public:

	ParticleSystem()
	{
	}

	void update(EntityManager & em, EventManager &evm, float delta);

};


class ParticleComponent {

public:

	ParticleComponent(uint16_t type) : m_particle_type(type) {
	}

	float m_life = 100000;
	uint16_t m_particle_type;
	uint16_t m_particle_id = 0;
};

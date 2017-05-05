#pragma once

#include <graphics/ParticleObject.hpp>
#include <graphics/GraphicsEngine.hpp>
#include <entities/System.hpp>

class ParticleSystem : public System<ParticleSystem>{

public:

	ParticleSystem()
	{
	}

	void update(EntityManager & em, EventManager &evm, float delta);

	static void pushRenderer(const pumpkin::ParticleRenderer & renderer) {
		rendererPool.push_back(renderer);
	}

};


class ParticleComponent {

public:

	ParticleComponent(uint16_t type) : m_renderer_id(type) {
	}

	float m_life = 100000;
	uint16_t m_particle_id = 0;
};

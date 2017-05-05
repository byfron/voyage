#pragma once

#include <graphics/ParticleObject.hpp>
#include <graphics/GraphicsEngine.hpp>

class ParticleComponent {

public:

	ParticleComponent(uint16_t type) : m_renderer_id(type) {
	}       

	float m_life = 100000;
	uint16_t m_renderer_id = 0;
	uint16_t m_particle_id = 0;
	pumpkin::ParticleGraphicsObject m_particle_graphics;
};

// TODO: This will have issues when avoiding graphics in the server
// We can divide in ParticleRenderSystem and ParticleSystem and
// ParticleGraphicsComponent

class ParticleSystem : public System<ParticleSystem>{

public:

	ParticleSystem(const std::vector<pumpkin::ParticleRenderer> & renderer_pool) :
		m_renderer_pool(renderer_pool) {
	}
	
	void update(EntityManager & em, EventManager &evm, float delta) {

		std::map<uint16_t, std::vector<pumpkin::ParticleGraphicsObject> > particle_pool;

		// Update particles
		em.each<ParticleComponent, BodyComponent>
			([delta, &particle_pool](Entity entity,
				 ParticleComponent &particle,
				 BodyComponent &body) {	


				// update all particles and gather them
				if (particle.m_life <= 0) {

					// delete entity?
					return;
				}

				particle.m_life -= delta;					
				particle.m_particle_graphics.m_transform = body.getTransform();
				body.m_position += body.m_moveVec;

				// send the particle to the corresponding renderer
				particle_pool[particle.m_renderer_id].push_back(particle.m_particle_graphics);
		});

		//std::cout << "particle pool:" << particle_pool.size() << std::endl;
			 
		// run through renderers
		for (auto & renderer : m_renderer_pool) {
			renderer.render(pumpkin::RENDER_PASS_GEOMETRY, particle_pool[renderer.id()]);
			particle_pool[renderer.id()].clear();
		}		 
	}

	void pushRenderer(const pumpkin::ParticleRenderer & renderer) {
		m_renderer_pool.push_back(renderer);
	}

	protected:
		
	std::vector<pumpkin::ParticleRenderer> m_renderer_pool;
};

#include "ParticleComponent.hpp"
#include "BodyComponent.hpp"
#include <game/CollisionManager.hpp>

void ParticleSystem::update(EntityManager & em, EventManager &evm, float delta) {

	std::vector<std::pair<Entity, ParticleComponent> > particles_to_destroy;

	// Update particles
	em.each<ParticleComponent, BodyComponent>
		([delta, &particles_to_destroy, this](Entity entity,
											 ParticleComponent &particle,
											 BodyComponent &body) {

			// update all particles and gather them
			if (particle.m_life <= 0) {
				particles_to_destroy.push_back(std::pair<Entity, ParticleComponent>(entity, particle));
			}
			else {
				particle.m_life -= delta;
				Vec3f move_vec = body.m_moveVec * body.m_moveSpeed * delta;
				if (CollisionManager::isCollidingWithWorld(m_world, body.m_position, move_vec)) {
					particles_to_destroy.push_back({entity, particle});
				}
				else {
					body.m_position += move_vec;
				}
			}

		});

	for (auto ent_part : particles_to_destroy) {

		// check component to send a message (sound, explosion etc...)
		// onDestroyed()

		em.destroy(ent_part.first.id());
	}
}

#pragma once

#include <pumpkin.hpp>
#include "CollisionComponent.hpp"
#include <entities/System.hpp>
#include "PlayerComponent.hpp"

class DebugGraphicsComponent {

public:
	void update(float delta) {
		m_debug.update(delta);
	}

	pumpkin::DebugGraphicsObject m_debug;

};

class ParticleGraphicsComponent {

public:

	ParticleGraphicsComponent(uint16_t type) : m_renderer_id(type) {
	}

	void render(float delta) {
		GraphicsSystem::particleRendererPool[m_renderer_id].push_back(render(pumpkin::RENDER_PASS_GEOMETRY, m_particle_graphics);
	}

	uint16_t m_renderer_id = 0;
	pumpkin::ParticleGraphicsObject m_particle;
};


class GraphicsComponent {
public:

	GraphicsComponent() {

	}

	void render(float delta) {
		m_graphics->render(delta);
	}

	pumpkin::GraphicsObject *m_graphics;
};

class GraphicsUpdateSystem : public System<GraphicsUpdateSystem> {
public:

	GraphicsSystem() {};

	void update(EntityManager & em, EventManager &evm, float delta ) {

		// update all graphic elements
		em.each<GraphicsComponent, BodyComponent>(
			[delta](Entity entity,
				GraphicsComponent &go,
				BodyComponent &body) {

				go.m_graphics->setTransform(body.getTransform());
				go.m_graphics->update(delta);
		});

		// update particle graphics
		em.each<ParticleGraphicsComponent, BodyComponent>(
			[delta](Entity entity,
				ParticleGraphicsComponent &go,
				BodyComponent &body) {

				go.m_particle.setTransform(body.getTransform());
				go.m_particle.update(delta);

				// push into particle renderer

		});



		// Draw visibility component (in the client there should be only one)



	}

	static std::vector<pumpkin::ParticleRenderer> particleRendererPool;

};


class DebugGraphicsSystem : public System<DebugGraphicsSystem> {
public:
	DebugGraphicsSystem() {
	}

	void update(EntityManager & em, EventManager &evm, float delta ) {

		em.each<DebugGraphicsComponent, CollisionComponent, BodyComponent>(
			[delta](Entity entity,
				DebugGraphicsComponent &debug,
				CollisionComponent &col,
				BodyComponent &body) {

			debug.update(delta);

			});
	}
};

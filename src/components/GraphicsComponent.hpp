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

class GraphicsComponent {
public:

	GraphicsComponent() {

	}

	void render(float delta) {
		m_graphics->render(delta);
	}

	pumpkin::GraphicsObject *m_graphics;
};

class GraphicsSystem : public System<GraphicsSystem> {
public:

	GraphicsSystem() {};

	void update(EntityManager & em, EventManager &evm, float delta ) {

		// Draw all graphic elements
		em.each<GraphicsComponent, BodyComponent>(
			[delta](Entity entity,
				GraphicsComponent &go,
				BodyComponent &body) {

				go.m_graphics->setTransform(body.getTransform());
				go.m_graphics->update(delta);
		});

		// Draw visibility component (in the client there should be only one)



	}

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

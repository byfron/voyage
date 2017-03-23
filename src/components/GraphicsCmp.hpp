#pragma once

#include <pumpkin.hpp>
#include "CollisionComponent.hpp"
#include <entities/System.hpp>
#include "PlayerSystem.hpp"

class DebugGraphicsCmp {

public:
	void update(float delta) {
		m_debug.update(delta);
	}

	pumpkin::DebugGraphicsObject m_debug;

};

class GraphicsCmp {
public:

	GraphicsCmp() {

	}

	pumpkin::GraphicsObject *m_graphics;
};

class GraphicsSystem : public System<GraphicsSystem> {
public:

	GraphicsSystem() {};

	void update(EntityManager & em, EventManager &evm, float delta ) {

		// Draw all graphic elements
		em.each<GraphicsCmp, BodyCmp>(
			[delta](Entity entity,
				GraphicsCmp &go,
				BodyCmp &body) {

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

		em.each<DebugGraphicsCmp, CollisionComponent, BodyCmp>(
			[delta](Entity entity,
				DebugGraphicsCmp &debug,
				CollisionComponent &col,
				BodyCmp &body) {


			debug.update(delta);

			});
	}
};

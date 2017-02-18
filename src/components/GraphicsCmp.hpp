#pragma once

#include <pumpkin.hpp>
#include "CollisionComponent.hpp"
#include <entities/System.hpp>
#include "PlayerSystem.hpp"

class pumpkin::PosColorVertex;

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
		m_graphics.init();
	}

	pumpkin::GraphicsObject<pumpkin::PosColorVertex> m_graphics;
};

class SpriteCmp {

public:

	// Same as animation but with a fixed atlas offset!
	SpriteCmp() {}

	pumpkin::SpriteObject m_graphics;
};

class GraphicsSystem : public System<GraphicsSystem> {
public:
	GraphicsSystem() {};

	void update(EntityManager & em, EventManager &evm, float delta ) {

		em.each<GraphicsCmp, BodyCmp>(
			[delta](Entity entity,
				GraphicsCmp &go,
				BodyCmp &body) {

				go.m_graphics.setTransform(body.getTransform());
				go.m_graphics.update(delta);
		});
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

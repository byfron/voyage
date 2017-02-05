#pragma once

#include <pumpkin.hpp>
#include "CollisionComponent.hpp"
#include <entities/System.hpp>

class DebugGraphicsCmp {

public:
	void update(float delta) {
		m_debug.update(delta);
	}
	
	pumpkin::DebugGraphicsObject m_debug;

};

class GraphicsCmp {
public:

	GraphicsCmp() {}

	pumpkin::GraphicsObject m_graphics;
};
	
class SpriteCmp {

public:

	// Same as animation but with a fixed atlas offset!
	SpriteCmp() {}

	pumpkin::SpriteObject m_graphics;
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

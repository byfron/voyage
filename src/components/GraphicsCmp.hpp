#pragma once

#include <pumpkin.hpp>
#include "CollisionComponent.hpp"
#include <graphics/DebugGraphicsObject.hpp>

class DebugGraphicsCmp {

public:
       
	void update(float delta) {
		m_debug.update(delta);
	}
	
	pumpkin::DebugGraphicsObject m_debug;

};

class GraphicsCmp {

public:

	// Same as animation but with a fixed atlas offset!
	GraphicsCmp() {}

	pumpkin::GraphicsObject m_graphics;
};


class DebugGraphicsSystem : public System<DebugGraphicsSystem> {
public:
	DebugGraphicsSystem() {
	}	

	void update(EntityManager & em, EventManager &evm, float delta ) {

		// run static debug utils
		pumpkin::DebugManager::update(delta);
		
		em.each<DebugGraphicsCmp, BodyCmp>(
			[delta](Entity entity,
				DebugGraphicsCmp &debug,
				BodyCmp &body) {		
			
			debug.m_debug.setAABB(body.m_aabb.m_min, body.m_aabb.m_max);
			debug.m_debug.setPosition(body.m_position);
			debug.m_debug.setRotation(body.m_rotation);			
			
			debug.update(delta);

			});
	}
};

#pragma once

#include <pumpkin.hpp>
#include "CollisionComponent.hpp"
#include <entities/System.hpp>
#include "PlayerComponent.hpp"
#include "PlayerInput.hpp"
#include <graphics/Animation.hpp>
#include <graphics/AnimationFactory.hpp>
#include <graphics/ParticleObject.hpp>
#include <graphics/GraphicsEngine.hpp>
#include "RenderSystem.hpp"

using namespace PlayerInput;

class DebugGraphicsComponent {

public:
	void update(float delta) {
		m_debug.update(delta);
	}

	pumpkin::DebugGraphicsObject m_debug;

};


// shall the graphic elements be organized per texture?
class AnimationComponent {
public:
	AnimationComponent(const pumpkin::Animation::Config & config) :
		m_animation(config) {
		m_animation.init();
	}

	// TODO: this should not be here
	void retrieveAnimation(int32_t action) {

		if (action & (1 << (int)Action::FACING_LEFT)) {
			m_animation.switchToAnim(6);
		}
		else if (action & (1 << (int)Action::FACING_RIGHT)) {
			m_animation.switchToAnim(2);
		}
		else if (action & (1 << (int)Action::FACING_UP)) {
			m_animation.switchToAnim(0);
		}
		else if (action & (1 << (int)Action::FACING_DOWN)) {
			m_animation.switchToAnim(4);
		}
		else if (action & (1 << (int)Action::FACING_DOWN_LEFT)) {
			m_animation.switchToAnim(7);
		}
		else if (action & (1 << (int)Action::FACING_DOWN_RIGHT)) {
			m_animation.switchToAnim(3);
		}
		else if (action & (1 << (int)Action::FACING_UP_LEFT)) {
			m_animation.switchToAnim(5);
		}
		else if (action & (1 << (int)Action::FACING_UP_RIGHT)) {
			m_animation.switchToAnim(1);
		}

		else m_animation.stop();
	}

	void render(float delta) {
		m_animation.render(delta);
	}

	void update(float delta ) {
		m_animation.update(delta);
	}

	pumpkin::Animation & getAnimation2D() {
		return m_animation;
	}

private:

	pumpkin::Animation m_animation;
};


class ParticleGraphicsComponent {

public:

	ParticleGraphicsComponent(uint16_t type) : m_renderer_id(type) {
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

	GraphicsUpdateSystem() {

		// initialize particle renderers

	};



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

				//go.m_particle.setTransform(body.getTransform());
				go.m_particle.update(delta);


				assert(RenderSystem::particleRendererPool.count(go.m_renderer_id));

				RenderSystem::particleRendererPool[go.m_renderer_id]->
					pushParticle(go.m_particle);
		});

		// Update animatoons
		em.each<AnimationComponent, BodyComponent>(
			[delta](Entity entity,
					AnimationComponent &anim,
					BodyComponent & body) {
				anim.getAnimation2D().setTransform(body.getTransform());
				anim.update(delta);
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

#pragma once

#include <graphics/Animation.hpp>
#include <graphics/AnimationFactory.hpp>

// shall the graphic elements be organized per texture?
class AnimationComponent {
public:
	AnimationComponent(const std::string & config_file) :
		m_animation(config_file) {
		m_animation.init();
	}

	void update(float delta ) {
		m_animation.update(delta);
	}

	pumpkin::Animation & getAnimation2D() {
		return m_animation;
	}

private:

	uint32_t m_resource_id;
	pumpkin::Animation m_animation;
};


class AnimationSystem : public System<AnimationSystem> {
public:
	void update(EntityManager & em, EventManager &evm, float delta ) {
		em.each<AnimationComponent, PlayerState>([delta](Entity entity,
								 AnimationComponent &anim,
								 PlayerState & state) {
//			 if (state.isDirty()) {
				 anim.getAnimation2D().setPosition(state.getPosition());
				 anim.getAnimation2D().switchToAnim(state.getAnimationId());
//			 }

			 anim.update(delta);
		 });
	}

private:

};

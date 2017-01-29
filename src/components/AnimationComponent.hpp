#pragma once

#include "BodyCmp.hpp"
#include "InputCmp.hpp"
#include <entities/System.hpp>
#include <graphics/Animation.hpp>
#include <graphics/AnimationFactory.hpp>


// shall the graphic elements be organized per texture?
class AnimationComponent {
public:
	AnimationComponent(const std::string & config_file) :
		m_animation(config_file) {
		m_animation.init();
	}

	void retrieveAnimation(int32_t action) {

		if (action &  (1 << (int)Action::FACING_LEFT)) {
			m_animation.switchToAnim(3);
		}
		else if (action & (1 << (int)Action::FACING_RIGHT)) {
			m_animation.switchToAnim(2);
		}
		
		if (action & (1 << (int)Action::FACING_UP)) {
			m_animation.switchToAnim(1);
		}
		else if (action & (1 << (int)Action::FACING_DOWN)) {
			m_animation.switchToAnim(0);
		}
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

	void updateAnimation(AnimationComponent &anim, BodyCmp & body) {

	}
	
	void update(EntityManager & em, EventManager &evm, float delta ) {
		em.each<AnimationComponent, BodyCmp>([delta](Entity entity,
							     AnimationComponent &anim,
							     BodyCmp & body) {
//			 if (state.isDirty()) {

			     anim.retrieveAnimation(body.m_action_id);		 
			     anim.getAnimation2D().setPosition(body.m_position);
			     anim.update(delta);
								 
//			 }
		 });	       
	}

private:

};

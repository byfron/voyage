#pragma once

#include "BodyCmp.hpp"
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
		
		m_animation.switchToAnim(0);
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

				// check in a table which animation shall we move to
                                // given the actionState
							    
			        // tell pumpkin to go to that one

				// set position of the graphical object
							    
							    
//				 updateAnimation(anim, body);
				 
				     anim.retrieveAnimation(body.m_action_id);		 
				     anim.getAnimation2D().setPosition(body.m_position);
				     anim.update(delta);
								 
//			 }

			 anim.update(delta);
		 });	       
	}

private:

};

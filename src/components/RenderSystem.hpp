#include "System.hpp"

class RenderSystem : public System<RenderSystem> {

public:
	RenderSystem() {}

	void updateVisibility(Vec2f player_position, const GameLevel & level) {

	}


	void update(EntityManager & em, EventManager &evm, float delta ) {

		//Set-up stencil buffer
		m_stencil.craftStencil(m_visibilityManager.getPolygon());

		//Set-up lights
		//m_lightManager

		//Draw everything
		render(em, delta);
	}


protected:

	pumpkin::Stencil m_stencil;
	VisibilityManager & m_visibilityManager;

};

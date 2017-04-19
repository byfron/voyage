#include "PlayerComponent.hpp"
#include "BodyComponent.hpp"

void PlayerSystem::update(EntityManager & em, EventManager &evm, float delta ) {
	
	em.each<NetworkComponent, PlayerComponent, BodyComponent>
		([&em, delta, this](Entity entity,
				    NetworkComponent &netdata,
				    PlayerComponent &player,
				    BodyComponent & body) {
			
		});
}

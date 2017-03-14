#include "PlayerSystem.hpp"
#include "BodyCmp.hpp"

void PlayerSystem::update(EntityManager & em, EventManager &evm, float delta ) {
	
	em.each<NetworkCmp, PlayerCmp, BodyCmp>
		([&em, delta, this](Entity entity,
				    NetworkCmp &netdata,
				    PlayerCmp &player,
				    BodyCmp & body) {
			
		});
}

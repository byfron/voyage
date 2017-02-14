#include <entities/System.hpp>
#include <pumpkin.hpp>

class MapComponent {
public:
	MapComponent(pumpkin::TileMap & tmap) : m_tileMap(tmap) {
	}
	void update(float delta ) {
		m_tileMap.update(delta);
	}

private:

	pumpkin::TileMap & m_tileMap;

};

class MapSystem : public System<MapSystem> {
public:
	void update(EntityManager & em, EventManager &evm, float delta ) {
		em.each<MapComponent>([delta](Entity entity,
					      MapComponent &map) {

			       //update graphics with player state
				map.update(delta);
		});
	}
};

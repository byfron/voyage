#include <entities/System.hpp>
#include <pumpkin.hpp>

// class MapComponent {
// public:
// 	MapComponent(pumpkin::TileMap & tmap) : m_tileMap(tmap) {
// 	}
// 	void update(float delta ) {
// 		m_tileMap.update(delta);
// 	}

// private:

// 	pumpkin::TileMap m_tileMap;

// };

class MapDrawSystem : public System<MapDrawSystem> {
public:

	MapDrawSystem(World::Ptr world) : _world(world) {
		//	loadTileMap();

		loadScene();
	}

	bool loadScene() {

		pumpkin::Configuration<pumpkin::Scene::Config> config
			(std::string(CONFIG_FILE_PATH) +
			 "scene.cfg");
		
		m_scene = std::make_shared<pumpkin::Scene>(config.config());
		FbxLoader loader("test2.fbx");
		m_scene->loadFromFbxNode(loader.getRootNode());
		m_scene->init();

		for (auto room : _world->getGameLevel().getRoomVector()) {
			for (auto poly : room.collision_polygons) {
//				m_scene->m_collision_polys.push_back(poly.vertices);
			}
		}
	}

	// bool loadTileMap() {

	// 	voyage::TileMapCfg  config =
	// 		_world->getGameMap()->getConfig().config();

	// 	// for (int i = 0; i < config.layers().size(); i++) {

	// 	// 	// voyage::TileMapLayerCfg layer_cfg = config.layers(i);

	// 	// 	// pumpkin::TileMapLayerProperties properties(
	// 	// 	// 	config.size_x(),
	// 	// 	// 	config.size_y(),
	// 	// 	// 	layer_cfg.atlas_id(),
	// 	// 	// 	layer_cfg.shader_id());

	// 	// 	// // Create Layer
	// 	// 	// pumpkin::TileMapLayer::Ptr layer = std::make_shared<pumpkin::TileMapLayer>(properties);

	// 	// 	// // Initialise internals before adding meshes
	// 	// 	// layer->init();

	// 	// 	//generateLayer(layer);

	// 	// 	//m_tilemap.addLayer(layer);
	// 	// }

	// 	return true;
	// }

	// void generateLayer(pumpkin::TileMapLayer::Ptr layer) {

	// 	float height = 0.5;
	// 	GameMap::TileMapData & data = _world->getGameMap()->tilemap_data();

	// 	for (int i = 0; i < _world->getGameMap()->size_x(); i++) {
	// 		for (int j = 0; j < _world->getGameMap()->size_y(); j++) {
	// 			layer->addMeshObject(i, j, data(i,j).type,
	// 					     _world->getGameMap()->tile_size(), height);
	// 		}
	// 	}

	// 	// We call this after the vertices are added
	// 	layer->initialiseBuffers();
	// }

	void update(EntityManager & em, EventManager &evm, float delta ) {


		m_scene->update(delta);


		//	em.each<MapComponent>([delta](Entity entity,
		//			      MapComponent &map) {

			       //update graphics with player state
		//m_tilemap.update(delta);

//		});
	}

	World::Ptr _world;
	pumpkin::Scene::Ptr m_scene;

	//pumpkin::TileMap m_tilemap;
};

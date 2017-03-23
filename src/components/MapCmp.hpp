#include <entities/System.hpp>
#include <pumpkin.hpp>

class MapDrawSystem : public System<MapDrawSystem> {
public:

	MapDrawSystem(World::Ptr world) : _world(world) {
		loadScene();
	}

	bool loadScene() {

		pumpkin::Configuration<pumpkin::Scene::Config> config
			(std::string(CONFIG_FILE_PATH) +
			 "scene.cfg");

		m_scene = std::make_shared<pumpkin::Scene>(config.config());
		std::string fbx_file = config.config().fbx_file();
		FbxLoader loader(std::string(SCENE_FILE_PATH) + fbx_file);
		m_scene->loadFromFbxNode(loader.getRootNode());
		m_scene->init();

		_world->loadGameLevel(loader);

		for (auto room : _world->getGameLevel().getRoomVector()) {
			for (auto poly : room.collision_polygons) {
//				m_scene->m_collision_polys.push_back(poly.vertices);
			}
		}
	}
	void update(EntityManager & em, EventManager &evm, float delta ) {

		for (auto room : _world->getGameLevel().getRoomVector()) {
			for (auto poly : room.collision_polygons) {
				pumpkin::DebugManager::push_polygon(poly.vertices);
			}
		}

		//m_scene->update(delta);
	}

	World::Ptr _world;
	pumpkin::Scene::Ptr m_scene;
};

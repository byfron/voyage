#include <entities/System.hpp>
#include <graphics/Stencil.hpp>

class SceneComponent {

public:
    SceneComponent(pumpkin::Configuration<pumpkin::Scene::Config> & config,
                   FbxLoader & loader) {

        m_scene = std::make_shared<pumpkin::Scene>(config.config());
        m_scene->loadFromFbxNode(loader.getRootNode());
        m_scene->init();
    }


    void render(float delta)
    {
        m_scene->render(delta);
    }
protected:

    pumpkin::Scene::Ptr m_scene;
};

class RenderSystem : public System<RenderSystem> {

public:
	RenderSystem(World::Ptr world, VisibilityManager &vm) :
            m_world(world),
            m_visibilityManager(vm) {}

	void updateVisibility(Vec2f player_position, const GameLevel & level) {

	}


	void update(EntityManager & em, EventManager &evm, float delta ) {

		//Set-up stencil buffer
//		m_stencil.craftStencil(m_visibilityManager.getPolygon());

		//Set-up lights
		//m_lightManager

		//Draw everything
		render(em, delta);
	}


	void render(EntityManager & em, float delta) {

        // Render graphics
        em.each<GraphicsCmp>(
                [delta](Entity entity,
                        GraphicsCmp &go) {
                    go.render(delta);
                });

        // Render animations
        em.each<AnimationComponent>(
                [delta](Entity entity,
                        AnimationComponent &ac) {
                    ac.render(delta);
                });

        // Render game level
        //renderScene(em, delta);

		// Render debug stuff
		m_visibilityManager.render(delta);

    }

    void renderScene(EntityManager & em, float delta) {

        em.each<SceneComponent>(
                [delta](Entity entity,
                        SceneComponent &sc) {
                    sc.render(delta);
                });

        for (auto room : m_world->getGameLevel().getRoomVector()) {
            for (auto poly : room.collision_polygons) {
                pumpkin::DebugManager::push_polygon(poly.vertices);
            }
        }
    }


protected:

    World::Ptr m_world;
	pumpkin::Stencil m_stencil;
	VisibilityManager & m_visibilityManager;

};

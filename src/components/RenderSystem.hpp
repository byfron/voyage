#include <entities/System.hpp>
#include <graphics/Stencil.hpp>
#include <graphics/GraphicsEngine.hpp>

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
		m_stencil.craftStencil(m_visibilityManager.getPolygon(),
							   m_visibilityManager.getPlayerPos());

		bgfx::setViewClear(pumpkin::RENDER_PASS_GEOMETRY,
				   BGFX_CLEAR_DEPTH,
				   0x30303000,   //rgba
				   1.0f,           //depth
				   (uint8_t) 0);   //stencil


		//draw everything (this breaks sometimes)
		render(em, delta);
	}

	void render(EntityManager & em, float delta) {


		// Render graphics
		em.each<GraphicsComponent>(
			[delta](Entity entity,
				GraphicsComponent &go) {
				go.render(delta);
			});

		// Render animations
		em.each<AnimationComponent>(
			[delta](Entity entity,
				AnimationComponent &ac) {
				ac.render(delta);
			});

		// Render game level
		renderScene(em, delta);

		// Render debug stuff
		//m_visibilityManager.render(delta);

	}

	void renderScene(EntityManager & em, float delta) {

        em.each<SceneComponent>(
                [delta](Entity entity,
                        SceneComponent &sc) {
                    sc.render(delta);
                });
    }


protected:

    World::Ptr m_world;
	pumpkin::Stencil m_stencil;
	VisibilityManager & m_visibilityManager;

};

#include "RenderSystem.hpp"
#include "GraphicsComponent.hpp"

// Initialize static pool of renderers
RenderSystem::ParticlePoolType
RenderSystem::particleRendererPool = {
	{0, std::make_shared<pumpkin::ParticleRenderer>(0)},
	{1, std::make_shared<pumpkin::ParticleRenderer>(1)}
};

SceneComponent:: SceneComponent(pumpkin::Configuration<pumpkin::Scene::Config> & config,
								FbxLoader & loader) {

	m_scene = std::make_shared<pumpkin::Scene>(config.config());
	m_scene->loadFromFbxNode(loader.getRootNode());
	m_scene->init();
}

void SceneComponent::render(float delta)
{
	m_scene->render(delta);
}


RenderSystem::RenderSystem(World::Ptr world, VisibilityManager &vm) :
	m_world(world),
	m_visibilityManager(vm) {

	// initialize resources in particleRenderers
	for (auto & pr : particleRendererPool) {
		pr.second->init();
	}

}

void RenderSystem::update(EntityManager & em, EventManager &evm, float delta ) {

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

void RenderSystem::render(EntityManager & em, float delta) {


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

	//Render particles
	renderParticles();

	//Render game level
	renderScene(em, delta);

	// Render debug stuff
	//m_visibilityManager.render(delta);

}

void RenderSystem::renderParticles() {

	for (auto & pr : particleRendererPool) {
		pr.second->render(pumpkin::RENDER_PASS_GEOMETRY);
	}

}

void RenderSystem::renderScene(EntityManager & em, float delta) {

	em.each<SceneComponent>(
		[delta](Entity entity,
				SceneComponent &sc) {
			sc.render(delta);
		});
}

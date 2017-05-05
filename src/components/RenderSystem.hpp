#pragma once

#include <entities/System.hpp>
#include <graphics/Stencil.hpp>
#include <graphics/GraphicsEngine.hpp>
#include <graphics/ParticleObject.hpp>
#include <visibility/VisibilityManager.hpp>

class SceneComponent {
public:
    SceneComponent(pumpkin::Configuration<pumpkin::Scene::Config> & config,
                   FbxLoader & loader);
    void render(float delta);

protected:
    pumpkin::Scene::Ptr m_scene;
};

class RenderSystem : public System<RenderSystem> {

public:

	typedef std::map<uint16_t, std::shared_ptr< pumpkin::ParticleRenderer> > ParticlePoolType;

	RenderSystem(World::Ptr world, VisibilityManager &vm);

	void update(EntityManager & em, EventManager &evm, float delta );
	void render(EntityManager & em, float delta);
	void renderParticles();
	void renderScene(EntityManager & em, float delta);

	static ParticlePoolType particleRendererPool;

protected:

    World::Ptr m_world;
	pumpkin::Stencil m_stencil;
	VisibilityManager & m_visibilityManager;

};

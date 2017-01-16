#pragma once

#include "Entity.hpp"
#include <memory>
#include <unordered_map>

class BaseSystem {

public:
	typedef size_t Family;
	virtual ~BaseSystem();

	virtual void update(EntityManager & em, /*EventManager &evm*/ float delta ) = 0;
	static Family _family_counter;
};

class System : public BaseSystem {
public:
	static Family family() {
		static Family family = _family_counter++;
		return family;
	}
	virtual ~System() {}
	
};

class SystemManager {

public:
	SystemManager(EntityManager &em
		      //EventManager *evm,
		) : _em(em) { _initialized = true; } //In case we need to set up something

	template <typename S>
	void add(std::shared_ptr<S> system) {
		_systems.insert(std::make_pair(S::family(), system));
	}

	template <typename S>
	std::shared_ptr<S> system() {
		auto it = _systems.find(S::family());
		assert(it != _systems.end());
		return it == _systems.end()
			? std::shared_ptr<S>()
			: std::shared_ptr<S>(std::static_pointer_cast<S>(it->second));
	}

	template <typename S>
	void update(float dt) {
		assert(_initialized && "SystemManager::configure() not called");
		std::shared_ptr<S> s = system<S>();
		s->update(_em, dt);//event_manager_, dt);
	}

	void update_all(float dt);

	typedef std::shared_ptr<SystemManager> Ptr;
	
private:

	bool _initialized = false;
	EntityManager &_em;
//	EventManager &event_manager_;
	std::unordered_map<BaseSystem::Family, std::shared_ptr<BaseSystem>> _systems;
	
};

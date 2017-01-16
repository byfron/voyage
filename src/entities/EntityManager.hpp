#pragma once

#include "Entity.hpp"

class EntityManager {

public:

	
	// TODO put this in namespace/class

	class IdUtils {
		
		uint32_t generateNewId() {
			Entity::Id id;
			id.index = _entity_count++;
			id.valid = true;
			return id;
		}
	
		uint32_t getFreeId() {
			assert(not _freeIdList.empty());
			uint32_t index = _freeIdList.back();
			_freeIdList.pop_back();
			return index;
		}

		void releaseId(uint32_t id) {
			_freeIdList.push_back(id);
		}

		uint32_t getMaxEntityCount() {
			return _entity_count;
		}
	};

	
public:	
	
	EntityManager() {};

	Entity create() {

		Entity::Id id;
		
		if (_freeIdList.empty()) {
			id = IdUtils::generateNewId();			
		}
		else {		    
			id = IdUtils::getFreeId();
		}
		
		Entity entity(this, id);		
		reserveEntityData(id.index());
		_valid_entities[id.index()] = true;
		
		//event_manager.broadcast<EntityCreatedEvent>(entity);
		return entity;
	}

	bool valid(Entity::Id id) {
		return _valid_entities[id.id];
	}
	
	void destroy(Entity::Id id) {
		assert(valid(id));
		IdUtils::releaseId(id.id);
		_valid_entities[id.id] = false;
	}

	Entity get(Entity::Id id) {
		assert(valid(id));
		return Entity(this, id);
	}
	
	template <typename CType>
	CType *getComponentPtr(Entity::Id id) {
		assert(valid(id));
		Pool *pool = getComponentPool<CType>();
		return pool->getComponentPtr<CType>(id);				
	}

	template <typename CType>
	Pool *getComponentPool() {
		Component::PoolIndex index = Component<CType>::poolIndex();
		assert(index < _component_pools.size());
		return _component_pools[index];
	}

	template <typename CType>
	std::vector<CType> & componentList() {
		Component::PoolIndex index = Component<CType>::poolIndex();
		return _component_pools[index];
	}
	
	template <typename C, typename ... Args>
	ComponentHandle<C> assign(Entity::Id id, Args && ... args) {
		valid(id);
		PoolIndex poolIndex = Component<C>::poolIndex();

//TODO: check (wth mask) that the entity does not have this component already

		BasePool *pool = reserveComponentData<c>();
		void *comp = pool->get(id.index());
		comp = new C(std::forward<Args>(args));

		//TODO: set bit in mask

		ComponentHandle<C> component(this, id);

		// TODO send message of comp. creation

		return component;
	}

	template <typename C>
	void remove(Entity::Id id) {
		valid(id);
		PoolIndex poolIndex = Component<C>::poolIndex();
		BasePool *pool = _component_pools[poolIndex];
	        pool->destroy(id);
	}

	
	
protected:

	void reserveEntityData(uint32_t id) {
		if (_valid_entities.size() <= id) {
			_valid_entities.resize(id+1);
			for (BasePool *pool : _component_pools)
				if (pool) pool->expand(id + 1);
		}
	}


	template <typename C>
	Pool<C>* reserveComponentData() {

		PoolIndex poolIndex = Component<C>::poolIndex();

		if (poolIndex > _component_pools.size()) {
			_component_pools.resize(poolIndex + 1);
		}

		if (not _component_pools[poolIndex]) {
			_component_pools[poolIndex] = new Pool<C>();
			_component_pools[poolIndex]->expand(EntityManager::getMaxEntityCount());
		}

		return static_cast<Pool<C>*>(_component_pools[poolIndex]);
	}
	
	std::vector<BasePool*> _component_pools;
	std::vector<ComponentMask> _entity_component_mask;
	std::vector<bool> _valid_entities;
        std::vector<uint32_t> _freeIdList;

};

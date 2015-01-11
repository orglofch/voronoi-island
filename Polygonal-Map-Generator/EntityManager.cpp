#include "EntityManager.hpp"

#include <algorithm>

#include "Entity.hpp"

EntityManager eEntityManager(1000);

EntityManager::EntityManager(size_t size)
{
	m_components.reserve(size);
	m_entities.reserve(size);
}

void EntityManager::Resize(size_t size)
{
	// TODO clean up components if size < current_size
	m_entities.resize(size);
	m_components.resize(size);
}
	
Entity &EntityManager::CreateEntity()
{
	// TODO make faster, use last known index etc.
	// Check for free dead entity
	for (Entity &entity : m_entities) {
		if (!entity.valid()) {
			entity.SetIsValid(true);
			return entity;
		}
	}

	m_entities.push_back(Entity());
	m_components.push_back(std::unordered_map<std::type_index, Component*>());
	m_entities.back().SetManager(this);
	m_entities.back().SetIsValid(true);
	return m_entities.back();
}

void EntityManager::DeleteEntity(const Entity &entity)
{
	m_entities.at(entity.id()).SetIsValid(false);
	m_components.at(entity.id()).clear();
}

void EntityManager::AddComponent(Component *c, const Entity *entity)
{
	m_components.at(entity->id())[std::type_index(typeid(*c))] = c;
}
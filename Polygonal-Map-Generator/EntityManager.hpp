#ifndef _ENTITY_MANAGER_HPP_
#define _ENTITY_MANAGER_HPP_

#include <unordered_map>
#include <typeindex>
#include <iostream>
#include <vector>

#include "Component.hpp"

class Entity;

class EntityManager
{
public:
	EntityManager(size_t size);

	void Resize(size_t size);

	Entity &CreateEntity();

	void DeleteEntity(const Entity &entity);

	void AddComponent(Component *c, const Entity *entity);

	template <typename T>
	bool HasComponent(const Entity *entity) const;

	template <typename T>
	void RemoveComponent(const Entity *entity);

	template <typename T>
	T *GetComponent(const Entity *entity);
	template <typename T>
	const T *GetComponent(const Entity *entity) const;

	std::vector<Entity> &entities() { return m_entities; }
	const std::vector<Entity> &entities() const { return m_entities; }

private:
	std::vector<std::unordered_map<std::type_index, Component*> > m_components;
	std::vector<Entity> m_entities;
};

extern EntityManager eEntityManager;

template <typename T>
inline bool EntityManager::HasComponent(const Entity *entity) const
{
	return m_components.at(entity->id()).count(std::type_index(typeid(T))) != 0;
}

template <typename T>
inline void EntityManager::RemoveComponent(const Entity *entity)
{
	return m_components.at(entity->id()).erase(m_components.at(entity->id()).find(std::type_index(typeid(T))));
}

template <typename T>
inline T *EntityManager::GetComponent(const Entity *entity)
{
	return static_cast<T*>(m_components.at(entity->id())[std::type_index(typeid(T))]);
}
template <typename T>
inline const T *EntityManager::GetComponent(const Entity *entity) const
{
	return static_cast<T*>(m_components.at(entity->id())[std::type_index(typeid(T))]);
}

#endif
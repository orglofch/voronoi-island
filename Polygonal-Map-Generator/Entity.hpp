#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

#include <vector>

#include "EntityManager.hpp"

struct Component;

class Entity
{
public:
	Entity() : 
		m_id(m_last_id++),
		m_valid(false),
		m_entity_manager(NULL)
	{}
	
	void SetManager(EntityManager *manager) { m_entity_manager = manager; }

	void AddComponent(Component *c);

	template <typename T>
	bool HasComponent() const;

	template <typename T>
	void RemoveComponent();

	template <typename T>
	T *GetComponent();
	template <typename T>
	const T *GetComponent() const;

	int id() const { return m_id; }

	bool valid() const { return m_valid; }
	void SetIsValid(bool valid) { m_valid = valid; }
	void Retire() { m_valid = false; }
	
private:
	static int m_last_id;

	int m_id;

	bool m_valid;

	EntityManager *m_entity_manager;
};

template <typename T>
inline void Entity::RemoveComponent() 
{
	m_entity_manager->RemoveComponent<T>(this);
}

template <typename T>
inline T *Entity::GetComponent() 
{
	return m_entity_manager->GetComponent<T>(this);
}

template <typename T>
inline const T *Entity::GetComponent() const 
{
	return m_entity_manager->GetComponent<T>(this);
}

template <typename T>
inline bool Entity::HasComponent() const 
{
	return m_entity_manager->HasComponent<T>(this);
}

#endif
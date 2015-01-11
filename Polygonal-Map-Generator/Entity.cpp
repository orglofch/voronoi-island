#include "Entity.hpp"

int Entity::m_last_id = 0;

void Entity::AddComponent(Component *c)
{
	m_entity_manager->AddComponent(c, this);
}
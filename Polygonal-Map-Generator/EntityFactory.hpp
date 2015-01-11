#ifndef _ENTITY_FACTORY_HPP_
#define _ENTITY_FACTORY_HPP_

class Entity;

enum EntityType
{
	ENTITY_PLAYER,
	ENTITY_COLLIDER,
};

class EntityFactory
{
public:
	static Entity &CreateEntity(const EntityType type);
};

#endif
#ifndef _COLLISION_SYSTEM_HPP_
#define _COLLISION_SYSTEM_HPP_

#include <vector>

#include "System.hpp"

class Entity;

class CollisionComponent;
class MovementComponent;

class CollisionSystem : public System
{
public:
	virtual void Update(const double deltaTime) override;

private:
	typedef std::vector<Entity> CollisionBucket;
	typedef std::pair<Entity, Entity> Collision;

	bool IsDynamic(const Entity &entity);

	std::vector<CollisionBucket> BroadPhase(const std::vector<Entity> &entities);
	std::vector<Collision> NarrowPhase(std::vector<CollisionBucket> &buckets);

	bool StaticIntersects(const CollisionComponent *c1, const CollisionComponent *c2);
	bool StaticDynamicIntersects(const CollisionComponent *c1, 
		const CollisionComponent *c2, const MovementComponent *m2);
	bool DynamicDynamicIntersects(const CollisionComponent *c1, const MovementComponent *m1,
		const CollisionComponent *c2, const MovementComponent *m2);
};

#endif
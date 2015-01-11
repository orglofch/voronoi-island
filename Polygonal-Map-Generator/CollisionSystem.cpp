#include "CollisionSystem.hpp"

#include "CollisionComponent.hpp"
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"
#include "MessageManager.hpp"
#include "EntityManager.hpp"
#include "SweptAABB.hpp"
#include "Entity.hpp"

using std::vector;
using std::pair;

void CollisionSystem::Update(const double deltaTime)
{
	std::vector<Entity> collision_entities;
	for (Entity &entity : eEntityManager.entities()) {
		if (entity.HasComponent<CollisionComponent>()) {
			collision_entities.push_back(entity);
		}
	}

	std::vector<Collision> collisions =
		NarrowPhase(BroadPhase(collision_entities));

	for (Collision &collision : collisions) {
		Entity &e1 = collision.first;
		Entity &e2 = collision.second;

		if (e1.HasComponent<MovementComponent>()) {
			e1.GetComponent<MovementComponent>()->velocity = Vector3D(0, 0, 0);
		}
		if (e2.HasComponent<MovementComponent>()) {
			e2.GetComponent<MovementComponent>()->velocity = Vector3D(0, 0, 0);
		}

		eMessageManager.BroadcastMessage(new CollisionMessage(e1, e2));
	}
}

vector<vector<Entity> > CollisionSystem::BroadPhase(
	const std::vector<Entity> &entities)
{
	std::vector<CollisionBucket> collision_buckets;

	collision_buckets.push_back(entities);

	return collision_buckets;
}

vector<pair<Entity, Entity> > CollisionSystem::NarrowPhase(
	vector<CollisionBucket> &buckets)
{
	std::vector<Collision> collisions;

	for (CollisionBucket &bucket : buckets) {
		size_t count = bucket.size();
		for (size_t i = 0; i < count; ++i) {
			Entity &e1 = bucket.at(i);
			CollisionComponent *c1 = e1.GetComponent<CollisionComponent>();
			c1->bounding_geometry->center() = e1.GetComponent<TransformComponent>()->position;
			bool e1_is_dynamic = IsDynamic(e1);
			for (size_t j = i + 1; j < count; ++j) {
				Entity &e2 = bucket.at(j);
				CollisionComponent *c2 = e2.GetComponent<CollisionComponent>();
				c2->bounding_geometry->center() = e2.GetComponent<TransformComponent>()->position;
				bool e2_is_dynamic = IsDynamic(e2);

				if (e1_is_dynamic && e2_is_dynamic) {
					if (DynamicDynamicIntersects(c1, e1.GetComponent<MovementComponent>(), 
						c2, e2.GetComponent<MovementComponent>())) {
						collisions.push_back(std::make_pair(e1, e2));
					}
				} else if (e1_is_dynamic) {
					if (StaticDynamicIntersects(c2, c1, e1.GetComponent<MovementComponent>())) {
						collisions.push_back(std::make_pair(e1, e2));
					}
				} else if (e2_is_dynamic) {
					if (StaticDynamicIntersects(c1, c2, e2.GetComponent<MovementComponent>())) {
						collisions.push_back(std::make_pair(e1, e2));
					}
				} else {
					if (StaticIntersects(c1, c2)) {
						collisions.push_back(std::make_pair(e1, e2));
					}
				}
			}
		}
	}

	return collisions;
}

bool CollisionSystem::IsDynamic(const Entity &entity)
{
	return entity.HasComponent<MovementComponent>();
}

bool CollisionSystem::StaticIntersects(const CollisionComponent *c1, const CollisionComponent *c2)
{
	return c1->bounding_geometry->Intersects(c2->bounding_geometry);
}

bool CollisionSystem::StaticDynamicIntersects(const CollisionComponent *c1,
	const CollisionComponent *c2, const MovementComponent *m2)
{
	// TODO
	return c1->bounding_geometry->Intersects(c2->bounding_geometry);
}

bool CollisionSystem::DynamicDynamicIntersects(const CollisionComponent *c1, const MovementComponent *m1,
	const CollisionComponent *c2, const MovementComponent *m2)
{
	// TODO
	return false;
}
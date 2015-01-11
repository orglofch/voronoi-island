#include "EntityFactory.hpp"

#include "EntityManager.hpp"
#include "Algebra.hpp"
#include "Entity.hpp"

#include "InteractableComponent.hpp"
#include "CollisionComponent.hpp"
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"
#include "TimelineComponent.hpp"
#include "DamageComponent.hpp"
#include "RenderComponent.hpp"
#include "InputComponent.hpp"
#include "AIComponent.hpp"

// TODO possibly move this to Entity Manager

Entity &EntityFactory::CreateEntity(const EntityType type)
{
	Entity &entity = eEntityManager.CreateEntity();
	switch (type) {
		case ENTITY_PLAYER:{
			TransformComponent *cTransform =
				new TransformComponent(Point3D(rand() % 640, rand() % 480, 0));
			MovementComponent *cMovement = new MovementComponent();
			cMovement->velocity = Vector3D(rand() / (float)RAND_MAX / 4, rand() / (float)RAND_MAX / 4, 0);
			AABB *aabb = new AABB();
			aabb->half_space() = Vector3D(1, 1, 1);
			aabb->center() = cTransform->position;
			CollisionComponent *cCollision = new CollisionComponent(aabb);

			entity.AddComponent(cTransform);
			entity.AddComponent(cMovement);
			entity.AddComponent(cCollision);
			entity.AddComponent(new RenderComponent());
			break;
		}
		case ENTITY_COLLIDER:
		{
			TransformComponent *cTransform = new TransformComponent(Point3D(200, 100, 0));
			AABB *aabb = new AABB();
			aabb->half_space() = Vector3D(1000, 1, 1);
			aabb->center() = cTransform->position;
			CollisionComponent *cCollision = new CollisionComponent(aabb);

			entity.AddComponent(cTransform);
			entity.AddComponent(cCollision);
			entity.AddComponent(new RenderComponent());
			break;
		}
	}
	return entity;
}
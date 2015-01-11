#include "MovementSystem.hpp"

#include "TransformComponent.hpp"
#include "MovementComponent.hpp"
#include "EntityManager.hpp"
#include "Algebra.hpp"
#include "Entity.hpp"

void MovementSystem::Update(const double deltaTime)
{
	for (Entity &entity : eEntityManager.entities()) {
		if (entity.HasComponent<MovementComponent>() && entity.HasComponent<TransformComponent>()) {
			MovementComponent *movementC = entity.GetComponent<MovementComponent>();
			TransformComponent *transformC = entity.GetComponent<TransformComponent>();
			
			transformC->matrix = translation(movementC->velocity * deltaTime) * transformC->matrix;
			transformC->inv_matrix = transformC->matrix.invert();
			transformC->position = transformC->matrix * Point3D(0, 0, 0);
			movementC->velocity += movementC->acceleration * deltaTime;
			movementC->velocity += Vector3D(0.0, -0.0005 * deltaTime, 0.0);
		}
	}
}

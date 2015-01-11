#include "LifeSystem.hpp"

#include "LifeComponent.hpp"
#include "EntityManager.hpp"
#include "Entity.hpp"

void LifeSystem::Update(const double deltaTime)
{
	for (Entity &entity : eEntityManager.entities()) {
		if (entity.HasComponent<LifeComponent>()) {
			LifeComponent *lifeC = entity.GetComponent<LifeComponent>();
			lifeC->life -= lifeC->decay_rate * deltaTime;
			if (lifeC->life < 0) {
				entity.Retire();
			}
		}
	}
}
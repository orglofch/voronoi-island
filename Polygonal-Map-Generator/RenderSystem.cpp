#include "RenderSystem.hpp"

#include "TransformComponent.hpp"
#include "RenderComponent.hpp"
#include "EntityManager.hpp"
#include "Entity.hpp"

#include "OpenGL.hpp"

void RenderSystem::Update(const double deltaTime)
{
	for (Entity &entity : eEntityManager.entities()) {
		if (entity.HasComponent<TransformComponent>() && entity.HasComponent<RenderComponent>()) {
			TransformComponent *cTransform = entity.GetComponent<TransformComponent>();
			RenderComponent *cRender = entity.GetComponent<RenderComponent>();

			glPushMatrix();
			{
				glMultMatrixd(cTransform->matrix.transpose().begin());
				glBegin(GL_TRIANGLE_STRIP);
				{
					glVertex3f(-2.0, -2.0, 0.0);
					glVertex3f(2.0, -2.0, 0.0);
					glVertex3f(-2.0, 2.0, 0.0);
					glVertex3f(2.0, 2.0, 0.0);
				}
				glEnd();
			}
			glPopMatrix();
		}
	}
}
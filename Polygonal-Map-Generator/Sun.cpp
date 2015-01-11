#include "Sun.hpp"

#include "OpenGL.hpp"

Sun::Sun(const Point3D &position) :
	m_transform(translation(position.AsVector()))
{
}

void Sun::Step(const double deltaTime)
{
	m_transform = rotation('x', 100 * deltaTime) * m_transform;

	const Point3D p = m_transform * Point3D(0, 0, 0);
	GLfloat lightPosition[] = { p[0], p[1], p[2], 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}
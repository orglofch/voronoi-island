#ifndef _AABB_HPP_
#define _AABB_HPP_

#include "BoundingGeometry.hpp"
#include "Assertion.hpp"

class AABB : public BoundingGeometry
{
public:
	AABB() : BoundingGeometry(BOUNDING_GEOMETRY_AABB) {}

	Vector3D &half_space() { return m_half_space; }
	const Vector3D &half_space() const { return m_half_space; }

	bool Contains(const Point3D &p);
	bool Intersects(const BoundingGeometry *other);
	bool IntersectsRay(const Ray &ray);

private:
	bool Intersects(const AABB *aabb);

	Vector3D m_half_space;
};

inline bool AABB::Contains(const Point3D &p)
{
	return !(p[0] < m_center[0] - m_half_space[0] ||
		p[0] > m_center[0] + m_half_space[0] ||
		p[1] < m_center[1] - m_half_space[1] ||
		p[1] > m_center[1] + m_half_space[1] ||
		p[2] < m_center[2] - m_half_space[2] ||
		p[2] > m_center[2] + m_half_space[2]);
}

inline bool AABB::Intersects(const BoundingGeometry *other)
{
	switch (other->type()) 
	{
		case BOUNDING_GEOMETRY_AABB:
			return Intersects(static_cast<const AABB*>(other));
		case BOUNDING_GEOMETRY_OBB:
		case BOUNDING_GEOMETRY_CIRCLE:
			break;
	}
	NOTREACHED();
	return false;
}

inline bool AABB::IntersectsRay(const Ray &ray)
{
	// TODO
	return false;
}

inline bool AABB::Intersects(const AABB *other)
{
	return (std::abs(m_center[0] - other->center()[0]) < m_half_space[0] + other->half_space()[0] &&
		std::abs(m_center[1] - other->center()[1]) < m_half_space[1] + other->half_space()[1] &&
		std::abs(m_center[2] - other->center()[2]) < m_half_space[2] + other->half_space()[2]);
}

#endif
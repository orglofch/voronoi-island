#ifndef _BOUNDING_GEOMETRY_HPP_
#define _BOUNDING_GEOMETRY_HPP_

#include "Algebra.hpp"
#include "Ray.hpp"

class BoundingGeometry
{
public:
	enum Type {
		BOUNDING_GEOMETRY_AABB,
		BOUNDING_GEOMETRY_OBB,
		BOUNDING_GEOMETRY_CIRCLE,
	};

	virtual bool Contains(const Point3D &p) = 0;
	virtual bool Intersects(const BoundingGeometry *other) = 0;
	virtual bool IntersectsRay(const Ray &ray) = 0;

	Type type() const { return m_type; }

	Point3D &center() { return m_center; }
	const Point3D &center() const { return m_center; }

protected:
	BoundingGeometry(Type type) : m_type(type) {}

	Type m_type;

	Point3D m_center;
};

#endif
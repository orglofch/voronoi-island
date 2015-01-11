#ifndef _OOB_HPP_
#define _OOB_HPP_

#include "BoundingGeometry.hpp"
#include "Assertion.hpp"

class OOB : public BoundingGeometry
{
public:
	OOB() : BoundingGeometry(BOUNDING_GEOMETRY_OBB) {}

	bool Contains(const Point3D &p);
	bool Intersects(const BoundingGeometry *other);
	bool IntersectsRay(const Ray &ray);

private:
};

inline bool OOB::Contains(const Point3D &p)
{
	// TODO
	return false;
}

inline bool OOB::Intersects(const BoundingGeometry *other)
{
	switch (other->type()) {
	case BOUNDING_GEOMETRY_AABB:
	case BOUNDING_GEOMETRY_OBB:
	case BOUNDING_GEOMETRY_CIRCLE:
		break;
	}
	NOTREACHED();
	return false;
}

inline bool OOB::IntersectsRay(const Ray &ray)
{
	// TODO
	return false;
}

#endif
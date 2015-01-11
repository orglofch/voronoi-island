#ifndef _COLLISION_COMPONENT_HPP_
#define _COLLISION_COMPONENT_HPP_

#include "BoundingGeometry.hpp"
#include "BoundingCircle.hpp"
#include "Algebra.hpp"
#include "AABB.hpp"
#include "OOB.hpp"

#include "Component.hpp"

struct CollisionComponent : public Component
{
	CollisionComponent(BoundingGeometry *_bounding_geometry)
		: bounding_geometry(_bounding_geometry) {}
	~CollisionComponent() {
		delete bounding_geometry;
	}

	enum Mask {
		PLAYER,
		ENEMY,
	};

	Mask mask;
	Mask valid_masks;

	BoundingGeometry *bounding_geometry;

	Point3D center_of_mass;
	double mass;
};

#endif
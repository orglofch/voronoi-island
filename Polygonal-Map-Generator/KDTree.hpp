#ifndef _KD_TREE_HPP_
#define _KD_TREE_HPP_

#include <cassert>
#include <vector>

#include "Algebra.hpp"

class KDTree
{
public:
	enum Orientation {
		HORIZONTAL,
		VERTICAL
	};

	KDTree(std::vector<Point3D> &points, size_t max_points = 3, Orientation orientation = HORIZONTAL);
	~KDTree() {
		if (!IsLeaf()) {
			delete m_tree_a;
			delete m_tree_b;
		}
	}

	Point3D Closest(const Point3D &p);

private:
	Point3D ClosestInSubTrees(const Point3D &p);

	bool IsLeaf() {
		return m_tree_a == NULL && m_tree_b == NULL;
	}

	Orientation m_orientation;

	Point3D m_divide;

	size_t m_max_points;
	std::vector<Point3D> m_points;

	KDTree *m_tree_a;
	KDTree *m_tree_b;
};

inline bool HorizontalComparison(const Point3D &p1, const Point3D &p2)
{
	return p1[0] < p2[0];
}

inline bool VerticalComparison(const Point3D &p1, const Point3D &p2)
{
	return p1[1] < p2[1];
}

inline KDTree::KDTree(std::vector<Point3D> &points, size_t max_points, Orientation orientation) :
	m_orientation(orientation),
	m_max_points(max_points),
	m_tree_a(NULL),
	m_tree_b(NULL)
{
	if (m_points.size() < m_max_points) {
		m_points = points;
	} else {
		std::sort(m_points.begin(), m_points.end(),
			m_orientation == HORIZONTAL ? VerticalComparison : HorizontalComparison);
		std::vector<Point3D>::iterator mid = points.begin() + floor(m_points.size() / 2);

		std::vector<Point3D> before(points.begin(), mid);
		std::vector<Point3D> after(mid + 1, points.end());
		m_divide = *mid;

		KDTree::Orientation new_orientation =
			m_orientation == HORIZONTAL ? VERTICAL : HORIZONTAL;

		// TODO maybe take advantage of presort
		m_tree_a = new KDTree(before, m_max_points, new_orientation);
		m_tree_b = new KDTree(after, m_max_points, new_orientation);
	}
}

inline Point3D KDTree::Closest(const Point3D &p)
{
	if (IsLeaf()) {
		double min_dist = std::numeric_limits<double>::infinity();
		int min_index = 0;
		for (int i = 0; i < m_points.size(); ++i) {
			double dist = (m_points.at(i) - p).length();
			if (dist < min_dist) {
				min_dist = dist;
				min_index = i;
			}
		}
		return m_points.at(min_index);
	} else {
		Point3D closest_in_subtrees = ClosestInSubTrees(p);
		return (closest_in_subtrees - p).length() < 
			(m_divide - p).length() ? closest_in_subtrees : m_divide;
	}
}

Point3D KDTree::ClosestInSubTrees(const Point3D &p)
{
	assert(!IsLeaf());
	KDTree *m_bucket;
	KDTree *m_other_bucket;
	switch (m_orientation) 
	{
		case HORIZONTAL:
			if (p[1] < m_divide[1]) {
				m_bucket = m_tree_a;
				m_other_bucket = m_tree_b;
			} else {
				m_bucket = m_tree_b;
				m_other_bucket = m_tree_a;
			}
			break;

		case VERTICAL:
			if (p[0] < m_divide[0]) {
				m_bucket = m_tree_a;
				m_other_bucket = m_tree_b;
			} else {
				m_bucket = m_tree_b;
				m_other_bucket = m_tree_a;
			}
			break;
	}
	Point3D closest = m_bucket->Closest(p);
	double dist = (p - closest).length();
	double distance_to_divide =
		m_orientation == HORIZONTAL ? abs(p[1] - m_divide[1]) : abs(p[0] - m_divide[0]);
	if (dist > distance_to_divide) {
		Point3D other_closest = m_other_bucket->Closest(p);
		double other_dist = (p - other_closest).length();
		return dist < other_dist ? closest : other_closest;
	}
	return closest;
}

#endif
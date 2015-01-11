#ifndef _QUADTREE_HPP_
#define _QUADTREE_HPP_

#include <vector>

#include "Algebra.hpp"

// TODO
template <class T>
class Quadtree
{
public:
	Quadtree();
	~Quadtree();

	template <typename T>
	void Insert(const T &t, const Point3D &p);

	bool IsLeaf() { return m_sub_trees[1] == NULL; }

	void Clear(void);

private:
	void Split(void);

	std::vector<std::pair<T, Point3D> > m_collection;

	Quadtree *m_sub_trees[4];
};

template <typename T>
inline Quadtree::Quadtree()
{
}

template <typename T>
inline Quadtree::~Quadtree()
{
}

template <typename T>
inline void Quadtree::Insert(const T &t, const Point3D &p)
{
}

template <typename T>
inline void Quadtree::Clear(void)
{
}

template <typename T>
inline void Quadtree::Split(void)
{
}

#endif
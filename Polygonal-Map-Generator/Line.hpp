#ifndef _LINE_HPP_
#define _LINE_HPP_

#include "Vector.hpp"
#include "Point.hpp"

class Line
{
public:
	Line(const Point3D &p1, const Point3D &p2)
	{
		m_p1 = p1;
		m_p2 = p2;
	}

	Point3D closest(const Point3D &p);

private:
	Point3D m_p1;
	Point3D m_p2;
};

inline Point3D Line::closest(const Point3D &p)
{
	Vector3D p1p2 = m_p2 - m_p1;
	Vector3D p1p = p - m_p1;
	p1p2.normalize();

	return m_p1 + p1p2.dot(p1p) * p1p2;
}

#endif
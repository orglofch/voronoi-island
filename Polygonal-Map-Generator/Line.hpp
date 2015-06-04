#ifndef _LINE_HPP_
#define _LINE_HPP_

#include "algebra.hpp"

class Line
{
public:
	Line(const Point3 &p1, const Point3 &p2)
	{
		m_p1 = p1;
		m_p2 = p2;
	}

	Point3 closest(const Point3 &p);

private:
	Point3 m_p1;
	Point3 m_p2;
};

inline Point3 Line::closest(const Point3 &p)
{
	Vector3 p1p2 = m_p2 - m_p1;
	Vector3 p1p = p - m_p1;
	p1p2.normalize();

	return m_p1 + p1p2.dot(p1p) * p1p2;
}

#endif
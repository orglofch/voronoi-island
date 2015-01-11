#ifndef _ALGEBRA_HPP_
#define _ALGEBRA_HPP_

#include <cmath>

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

#define EPSILON     0.0001
#define TO_RAD(a)   a * M_PI / 180.0
#define TO_DEG(a)	a * 180.0 / M_PI;

#include "Matrix4x4.hpp"
#include "Vector.hpp"
#include "Point.hpp"

inline Vector3D Point3D::AsVector() const
{
	return Vector3D(v_[0], v_[1], v_[2]);
}

inline Point3D Vector3D::AsPoint() const
{
	return Point3D(v_[0], v_[1], v_[2]);
}

inline Vector3D operator *(double s, const Vector3D& v)
{
  return Vector3D(s*v[0], s*v[1], s*v[2]);
}

inline Point3D operator +(const Point3D& a, const Vector3D& b)
{
  return Point3D(a[0]+b[0], a[1]+b[1], a[2]+b[2]);
}

inline Vector3D operator -(const Point3D& a, const Point3D& b)
{
  return Vector3D(a[0]-b[0], a[1]-b[1], a[2]-b[2]);
}

inline Vector3D operator -(const Vector3D& a)
{
  return Vector3D(-a[0], -a[1], -a[2]);
}

inline Point3D operator -(const Point3D& a, const Vector3D& b)
{
  return Point3D(a[0]-b[0], a[1]-b[1], a[2]-b[2]);
}

inline Vector3D operator *(const Matrix4x4& M, const Vector3D& v)
{
  return Vector3D(
                  v[0] * M[0][0] + v[1] * M[0][1] + v[2] * M[0][2],
                  v[0] * M[1][0] + v[1] * M[1][1] + v[2] * M[1][2],
                  v[0] * M[2][0] + v[1] * M[2][1] + v[2] * M[2][2]);
}

inline Point3D operator *(const Matrix4x4& M, const Point3D& p)
{
  return Point3D(
                 p[0] * M[0][0] + p[1] * M[0][1] + p[2] * M[0][2] + M[0][3],
                 p[0] * M[1][0] + p[1] * M[1][1] + p[2] * M[1][2] + M[1][3],
                 p[0] * M[2][0] + p[1] * M[2][1] + p[2] * M[2][2] + M[2][3]);
}

inline Vector3D transNorm(const Matrix4x4& M, const Vector3D& n)
{
  return Vector3D(
                  n[0] * M[0][0] + n[1] * M[1][0] + n[2] * M[2][0],
                  n[0] * M[0][1] + n[1] * M[1][1] + n[2] * M[2][1],
                  n[0] * M[0][2] + n[1] * M[1][2] + n[2] * M[2][2]);
}

inline Matrix4x4 rotation(const char axis, const double angle)
{
    switch (axis) 
    {
        case 'x': case 'X':
            return Matrix4x4(Vector4D(1, 0, 0, 0),
                             Vector4D(0, cos(TO_RAD(angle)), -sin(TO_RAD(angle)), 0),
                             Vector4D(0, sin(TO_RAD(angle)), cos(TO_RAD(angle)), 0),
                             Vector4D(0, 0, 0, 1));

        case 'y': case 'Y':
            return Matrix4x4(Vector4D(cos(TO_RAD(angle)), 0, sin(TO_RAD(angle)), 0),
                             Vector4D(0, 1, 0, 0),
                             Vector4D(-sin(TO_RAD(angle)), 0, cos(TO_RAD(angle)), 0),
                             Vector4D(0, 0, 0, 1));

        case 'z': case 'Z':
            return Matrix4x4(Vector4D(cos(TO_RAD(angle)), -sin(TO_RAD(angle)), 0, 0),
                             Vector4D(sin(TO_RAD(angle)), cos(TO_RAD(angle)), 0, 0),
                             Vector4D(0, 0, 1, 0),
                             Vector4D(0, 0, 0, 1));

        default:
            return Matrix4x4();
    }
}

#endif
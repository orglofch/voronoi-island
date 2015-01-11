#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include <iostream>
#include <cmath>

class Point3D;

class Vector2D
{
public:
  Vector2D()
  {
    v_[0] = 0.0;
    v_[1] = 0.0;
  }
  Vector2D(double x, double y)
  { 
    v_[0] = x;
    v_[1] = y;
  }
  Vector2D(const Vector2D& other)
  {
    v_[0] = other.v_[0];
    v_[1] = other.v_[1];
  }

  Vector2D& operator =(const Vector2D& other)
  {
    v_[0] = other.v_[0];
    v_[1] = other.v_[1];
    return *this;
  }

  double& operator[](size_t idx) 
  {
    return v_[ idx ];
  }
  double operator[](size_t idx) const 
  {
    return v_[ idx ];
  }

  double dot(const Vector2D& other) const
  {
    return v_[0]*other.v_[0] + v_[1]*other.v_[1];
  }

  double length2() const
  {
    return v_[0]*v_[0] + v_[1]*v_[1];
  }
  double length() const
  {
    return sqrt(length2());
  }

  double normalize();

  static Vector2D ZeroVector() { return Vector2D(0, 0); }

private:
  double v_[2];
};

inline Vector2D operator *(const Vector2D &v, const int s) {
	return Vector2D(v[0] * s, v[1] * s);
}

inline Vector2D operator *(const int s, const Vector2D &v) {
	return Vector2D(v[0] * s, v[1] * s);
}

inline Vector2D operator /(const Vector2D &v, const int s) {
	return Vector2D(v[0] / s, v[1] / s);
}

inline Vector2D operator +(const Vector2D& a, const Vector2D& b)
{
  return Vector2D(a[0]+b[0], a[1]+b[1]);
}

inline Vector2D operator -(const Vector2D& a, const Vector2D& b)
{
  return Vector2D(a[0]-b[0], a[1]-b[1]);
}

inline std::ostream& operator <<(std::ostream& os, const Vector2D& v)
{
  return os << "v<" << v[0] << "," << v[1] << ">";
}

class Vector3D
{
public:
  Vector3D()
  {
    v_[0] = 0.0;
    v_[1] = 0.0;
    v_[2] = 0.0;
  }
  Vector3D(double x, double y, double z)
  { 
    v_[0] = x;
    v_[1] = y;
    v_[2] = z;
  }
  Vector3D(const Vector3D& other)
  {
    v_[0] = other.v_[0];
    v_[1] = other.v_[1];
    v_[2] = other.v_[2];
  }

  Vector3D& operator =(const Vector3D& other)
  {
    v_[0] = other.v_[0];
    v_[1] = other.v_[1];
    v_[2] = other.v_[2];
    return *this;
  }

  double& operator[](size_t idx) 
  {
    return v_[ idx ];
  }
  double operator[](size_t idx) const 
  {
    return v_[ idx ];
  }
  void operator+=(const Vector3D &vec) 
  {
	  v_[0] += vec[0];
	  v_[1] += vec[1];
	  v_[2] += vec[2];
  }
  bool operator==(const Vector3D &vec) 
  {
	  return v_[0] == vec[0] &&
		  v_[1] == vec[1] &&
		  v_[2] == vec[2];
  }
  bool operator>(const Vector3D &vec)
  {
	  return v_[0] > vec[0] &&
		  v_[1] > vec[1] &&
		  v_[2] > vec[2];
  }
  bool operator<(const Vector3D &vec)
  {
	  return v_[0] < vec[0] &&
		  v_[1] < vec[1] &&
		  v_[2] < vec[2];
  }

  double dot(const Vector3D& other) const
  {
    return v_[0]*other.v_[0] + v_[1]*other.v_[1] + v_[2]*other.v_[2];
  }

  double length2() const
  {
    return v_[0]*v_[0] + v_[1]*v_[1] + v_[2]*v_[2];
  }
  double length() const
  {
    return sqrt(length2());
  }

  double normalize();

  Vector3D cross(const Vector3D& other) const
  {
    return Vector3D(
                    v_[1]*other[2] - v_[2]*other[1],
                    v_[2]*other[0] - v_[0]*other[2],
                    v_[0]*other[1] - v_[1]*other[0]);
  }

  const double *begin() const
  {
    return (double*)v_;
  }

  Point3D AsPoint() const;

  static Vector3D ZeroVector() { return Vector3D(0, 0, 0); }

private:
  double v_[3];
};

inline Vector3D cross(const Vector3D& a, const Vector3D& b) 
{
  return a.cross(b);
}

inline Vector3D operator *(const Vector3D &v, const int s) {
	return Vector3D(v[0] * s, v[1] * s, v[2] * s);
}

inline Vector3D operator *(const int s, const Vector3D &v) {
	return Vector3D(v[0] * s, v[1] * s, v[2] * s);
}

inline Vector3D operator /(const Vector3D &v, const int s) {
	return Vector3D(v[0] / s, v[1] / s, v[2] / s);
}

inline Vector3D operator +(const Vector3D& a, const Vector3D& b)
{
  return Vector3D(a[0]+b[0], a[1]+b[1], a[2]+b[2]);
}

inline Vector3D operator -(const Vector3D& a, const Vector3D& b)
{
  return Vector3D(a[0]-b[0], a[1]-b[1], a[2]-b[2]);
}

inline std::ostream& operator <<(std::ostream& os, const Vector3D& v)
{
  return os << "v<" << v[0] << "," << v[1] << "," << v[2] << ">";
}

class Vector4D
{
public:
  Vector4D()
  {
    v_[0] = 0.0;
    v_[1] = 0.0;
    v_[2] = 0.0;
    v_[3] = 0.0;
  }
  Vector4D(double x, double y, double z, double w)
  { 
    v_[0] = x;
    v_[1] = y;
    v_[2] = z;
    v_[3] = w;
  }
  Vector4D(const Vector4D& other)
  {
    v_[0] = other.v_[0];
    v_[1] = other.v_[1];
    v_[2] = other.v_[2];
    v_[3] = other.v_[3];
  }

  Vector4D& operator =(const Vector4D& other)
  {
    v_[0] = other.v_[0];
    v_[1] = other.v_[1];
    v_[2] = other.v_[2];
    v_[3] = other.v_[3];
    return *this;
  }

  double& operator[](size_t idx) 
  {
    return v_[ idx ];
  }
  double operator[](size_t idx) const 
  {
    return v_[ idx ];
  }

  const double *begin() const
  {
    return (double*)v_;
  }

private:
  double v_[4];
};

#endif
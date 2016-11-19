#ifndef _ALGEBRA_HPP_
#define _ALGEBRA_HPP_

#include <algorithm>
#include <cmath>

#ifndef PI
#define PI		3.14159265358979323846
#endif

#define EPSILON     0.00001
#define TO_RAD(a)   a * PI / 180.0
#define TO_DEG(r)	r * 180.0 / PI;

// TODO(orglofch): Possible combine Vector and Point
union Vector2
{
	Vector2() {
		x = y = 0;
	}
	Vector2(float _x, float _y) {
		x = _x;
		y = _y;
	}

	double length() {
		return sqrt(x*x + y*y);
	}

	float dot(const Vector2 &other) const {
		return x * other.x + y * other.y;
	}

	float &operator [] (int i) {
		return d[i];
	}

	float operator [] (int i) const {
		return d[i];
	}

	void operator += (const Vector2 &other) {
		x += other.x;
		y += other.y;
	}

	Vector2 operator /= (const float s) {
		x /= s;
		y /= s;

		return *this;
	}

	Vector2 operator *= (const float s) {
		x *= s;
		y *= s;

		return *this;
	}

	struct
	{
		float x, y;
	};
	struct
	{
		float u, v;
	};
private:
	float d[2];
};

inline
Vector2 operator - (const Vector2 &a, const Vector2 &b) {
	return Vector2(a.x - b.x, a.y - b.y);
}

inline
Vector2 operator / (const Vector2 &a, const float s) {
	return Vector2(a.x / s, a.y / s);
}

union Point2
{
	Point2() {
		x = y = 0;
	}
	Point2(float _x, float _y) {
		x = _x;
		y = _y;
	}

	Point2 &operator=(const Point2 &other) {
		x = other.x;
		y = other.y;

		return *this;
	}

	float &operator [] (int i) {
		return d[i];
	}

	float operator [] (int i) const {
		return d[i];
	}

	void operator += (const Vector2 &v) {
		x += v.x;
		y += v.y;
	}

	struct
	{
		float x, y;
	};
private:
	float d[2];
};

inline
Vector2 operator - (const Point2 &a, const Point2 &b) {
	return Vector2(a.x - b.x, a.y - b.y);
}

union Vector3
{
	Vector3() {
		x = y = z = 0;
	}
	Vector3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	double length() {
		return sqrt(x*x + y*y + z*z);
	}

	float dot(const Vector3 &other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	Vector3 cross(const Vector3 &other) const {
		return Vector3(y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x);
	}

	// TODO(orglofch): Remove / Replace
	const float *begin() const {
		return (float*)d;
	}

	float normalize() {
		float denom = 1.0f;
		float x_ = (x > 0) ? x : -x;
		float y_ = (y > 0) ? y : -y;
		float z_ = (z > 0) ? z : -z;

		if (x_ > y_) {
			if (x_ > z_) {
				if (1.0f + x_ > 1.0f) {
					y_ = y_ / x_;
					z_ = z_ / x_;
					denom = 1.0f / (x_ * sqrt(1.0f + y_*y_ + z_*z_));
				}
			} else { /* z > x > y */
				if (1.0f + z_ > 1.0f) {
					y_ = y_ / z_;
					x_ = x_ / z_;
					denom = 1.0f / (z_ * sqrt(1.0f + y_*y_ + x_*x_));
				}
			}
		} else {
			if (y_ > z_) {
				if (1.0f + y_ > 1.0f) {
					z_ = z_ / y_;
					x_ = x_ / y_;
					denom = 1.0f / (y_ * sqrt(1.0f + z_*z_ + x_*x_));
				}
			} else { /* x < y < z */
				if (1.0f + z_ > 1.0f) {
					y_ = y_ / z_;
					x_ = x_ / z_;
					denom = 1.0f / (z_ * sqrt(1.0f + y_*y_ + x_*x_));
				}
			}
		}

		if (1.0f + x_ + y_ + z_ > 1.0f) {
			x *= denom;
			y *= denom;
			z *= denom;
			return 1.0f / denom;
		}
		return 0;
	}

	float &operator [] (int i) {
		return d[i];
	}

	float operator [] (int i) const {
		return d[i];
	}

	void operator += (const Vector3 &other) {
		x += other.x;
		y += other.y;
		z += other.z;
	}

	Vector3 operator /= (const float s) {
		x /= s;
		y /= s;
		z /= s;

		return *this;
	}

	Vector3 operator *= (const float s) {
		x *= s;
		y *= s;
		z *= s;

		return *this;
	}

	struct
	{
		float x, y, z;
	};
	struct
	{
		float u, v, w;
	};
	struct
	{
		Vector2 xy;
		float UNUSED_0;
	};
	struct
	{
		Vector2 uv;
		float UNUSED_1;
	};
	struct
	{
		float UNUSED_2;
		Vector2 yz;
	};
	struct
	{
		float UNUSED_2;
		Vector2 vw;
	};
private:
	float d[3];
};

inline
Vector3 operator / (const Vector3 &v, const float s) {
	return Vector3(v.x / s, v.y / s, v.z / s);
}

inline
Vector3 operator * (const Vector3 &v, const float s) {
	return Vector3(v.x * s, v.y * s, v.z * s);
}

union Point3
{
	Point3() {
		x = y = z = 0;
	}
	Point3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	// TODO(orglofch): Remove / Replace
	const float *begin() const {
		return (float*)d;
	}

	Point3 &operator=(const Point3 &other) {
		x = other.x;
		y = other.y;
		z = other.z;

		return *this;
	}

	float &operator [] (int i) {
		return d[i];
	}

	float operator [] (int i) const {
		return d[i];
	}

	void operator += (const Vector3 &v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}

	void operator += (const Vector2 &v) {
		x += v.x;
		y += v.y;
	}

	struct
	{
		float x, y, z;
	};
	struct
	{
		Vector2 xy;
		float UNUSED_0;
	};
	struct
	{
		float UNUSED_1;
		Vector2 yz;
	};
private:
	float d[3];
};

inline
Vector3 operator - (const Point3 &a, const Point3 &b) {
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

// TODO(orglofch): Remove
inline
Point3 operator + (const Point3 &a, const Point3 &b) {
	return Point3(a.x + b.x, a.y + b.y, a.z + b.z);
}

// TODO(orglofch): Remove
inline
Point3 operator / (const Point3 &p, const float s) {
	return Point3(p.x / s, p.y / s, p.z / s);
}

union Vector4
{
	Vector4() {
		x = y = z = w = 0;
	}
	Vector4(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	float &operator [] (int i) {
		return d[i];
	}

	float operator [] (int i) const {
		return d[i];
	}

	struct
	{
		float x, y, z, w;
	};
	struct
	{
		Vector3 xyz;
		float UNUSED_0;
	};
	struct
	{
		float UNUSED_1;
		Vector3 yzw;
	};
private:
	float d[4];
};

// TODO(orglofch): Update
class Matrix4x4
{
public:
	Matrix4x4() {
		std::fill(v_, v_ + 16, 0.0);
		v_[0] = 1.0;
		v_[5] = 1.0;
		v_[10] = 1.0;
		v_[15] = 1.0;
	}
	Matrix4x4(const Matrix4x4& other) {
		std::copy(other.v_, other.v_ + 16, v_);
	}
	Matrix4x4(const Vector4 row1, const Vector4 row2, const Vector4 row3,
		const Vector4 row4) {
		v_[0] = row1[0];
		v_[1] = row1[1];
		v_[2] = row1[2];
		v_[3] = row1[3];

		v_[4] = row2[0];
		v_[5] = row2[1];
		v_[6] = row2[2];
		v_[7] = row2[3];

		v_[8] = row3[0];
		v_[9] = row3[1];
		v_[10] = row3[2];
		v_[11] = row3[3];

		v_[12] = row4[0];
		v_[13] = row4[1];
		v_[14] = row4[2];
		v_[15] = row4[3];
	}
	Matrix4x4(double *vals) {
		std::copy(vals, vals + 16, (double*)v_);
	}

	Matrix4x4& operator=(const Matrix4x4& other) {
		std::copy(other.v_, other.v_ + 16, v_);
		return *this;
	}

	Vector4 getRow(size_t row) const {
		return Vector4(v_[4 * row], v_[4 * row + 1], v_[4 * row + 2], v_[4 * row + 3]);
	}
	double *getRow(size_t row) {
		return (double*)v_ + 4 * row;
	}

	Vector4 getColumn(size_t col) const {
		return Vector4(v_[col], v_[4 + col], v_[8 + col], v_[12 + col]);
	}

	Vector4 operator[](size_t row) const {
		return getRow(row);
	}
	double *operator[](size_t row) {
		return getRow(row);
	}

	Matrix4x4 transpose() const {
		return Matrix4x4(getColumn(0), getColumn(1),
			getColumn(2), getColumn(3));
	}
	Matrix4x4 invert() const;

	const double *begin() const {
		return (double*)v_;
	}
	const double *end() const {
		return begin() + 16;
	}

private:
	double v_[16];
};

inline 
Matrix4x4 operator *(const Matrix4x4& a, const Matrix4x4& b) {
	Matrix4x4 ret;

	for (size_t i = 0; i < 4; ++i) {
		Vector4 row = a.getRow(i);

		for (size_t j = 0; j < 4; ++j) {
			ret[i][j] = row[0] * b[0][j] + row[1] * b[1][j] +
				row[2] * b[2][j] + row[3] * b[3][j];
		}
	}

	return ret;
}

#endif
#ifndef _MATRIX_4X4_HPP_
#define _MATRIX_4X4_HPP_

#include <algorithm>
#include <iostream>

#include "Vector.hpp"

class Matrix4x4
{
public:
  Matrix4x4()
  {
    // Construct an identity matrix
    std::fill(v_, v_+16, 0.0);
    v_[0] = 1.0;
    v_[5] = 1.0;
    v_[10] = 1.0;
    v_[15] = 1.0;
  }
  Matrix4x4(const Matrix4x4& other)
  {
    std::copy(other.v_, other.v_+16, v_);
  }
  Matrix4x4(const Vector4D row1, const Vector4D row2, const Vector4D row3, 
             const Vector4D row4)
  {
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
  Matrix4x4(double *vals)
  {
    std::copy(vals, vals + 16, (double*)v_);
  }

  Matrix4x4& operator=(const Matrix4x4& other)
  {
    std::copy(other.v_, other.v_+16, v_);
    return *this;
  }

  Vector4D getRow(size_t row) const
  {
    return Vector4D(v_[4*row], v_[4*row+1], v_[4*row+2], v_[4*row+3]);
  }
  double *getRow(size_t row) 
  {
    return (double*)v_ + 4*row;
  }

  Vector4D getColumn(size_t col) const
  {
    return Vector4D(v_[col], v_[4+col], v_[8+col], v_[12+col]);
  }

  Vector4D operator[](size_t row) const
  {
    return getRow(row);
  }
  double *operator[](size_t row) 
  {
    return getRow(row);
  }

  Matrix4x4 transpose() const
  {
    return Matrix4x4(getColumn(0), getColumn(1), 
                      getColumn(2), getColumn(3));
  }
  Matrix4x4 invert() const;

  const double *begin() const
  {
    return (double*)v_;
  }
  const double *end() const
  {
    return begin() + 16;
  }

private:
  double v_[16];
};

inline Matrix4x4 operator *(const Matrix4x4& a, const Matrix4x4& b)
{
  Matrix4x4 ret;

  for(size_t i = 0; i < 4; ++i) {
    Vector4D row = a.getRow(i);
		
    for(size_t j = 0; j < 4; ++j) {
      ret[i][j] = row[0] * b[0][j] + row[1] * b[1][j] + 
        row[2] * b[2][j] + row[3] * b[3][j];
    }
  }

  return ret;
}

inline Matrix4x4 rotation(const Vector3D &view, const Vector3D &up)
{
    Vector3D w = -1 * view;
    w.normalize();

    Vector3D u = (up.cross(w));
    u.normalize();

    Vector3D v = (-1 * w).cross(u);

    return Matrix4x4(Vector4D(u[0], v[0], w[0], 0),
                     Vector4D(u[1], v[1], w[1], 0),
                     Vector4D(u[2], v[2], w[2], 0),
                     Vector4D(0,    0,    0,    1));
}

inline Matrix4x4 translation(const Vector3D &amount)
{
    return Matrix4x4(Vector4D(1, 0, 0, amount[0]),
                     Vector4D(0, 1, 0, amount[1]),
                     Vector4D(0, 0, 1, amount[2]),
                     Vector4D(0, 0, 0, 1));
}

inline Matrix4x4 scaling(const Vector3D &amount)
{
    return Matrix4x4(Vector4D(amount[0], 0, 0, 0),
                     Vector4D(0, amount[1], 0, 0),
                     Vector4D(0, 0, amount[2], 0),
                     Vector4D(0, 0,         0, 1));
}

inline std::ostream& operator <<(std::ostream& os, const Matrix4x4& M)
{
  return os << "[" << M[0][0] << " " << M[0][1] << " " 
            << M[0][2] << " " << M[0][3] << "]" << std::endl
            << "[" << M[1][0] << " " << M[1][1] << " " 
            << M[1][2] << " " << M[1][3] << "]" << std::endl
            << "[" << M[2][0] << " " << M[2][1] << " " 
            << M[2][2] << " " << M[2][3] << "]" << std::endl
            << "[" << M[3][0] << " " << M[3][1] << " " 
            << M[3][2] << " " << M[3][3] << "]";
}

#endif
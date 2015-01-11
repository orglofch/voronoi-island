#ifndef _COLOUR_HPP_
#define _COLOUR_HPP_

#include <iostream>

class Colour
{
public:
  Colour()
  {
	  c_[0] = c_[1] = c_[2] = c_[3] = 0;
  }
  Colour(double r, double g, double b)
  {
    c_[0] = r;
    c_[1] = g;
    c_[2] = b;
	c_[3] = 1.0f;
  }
  Colour(double r, double g, double b, double a)
  {
    c_[0] = r;
    c_[1] = g;
    c_[2] = b;
	c_[3] = a;
  }
  Colour(double c)
  {
    c_[0] = c_[1] = c_[2] = c_[3] = c;
  }
  Colour(const Colour& other)
  {
    c_[0] = other.c_[0];
    c_[1] = other.c_[1];
    c_[2] = other.c_[2];
	c_[3] = other.c_[3];
  }

  Colour& operator =(const Colour& other)
  {
    c_[0] = other.c_[0];
    c_[1] = other.c_[1];
    c_[2] = other.c_[2];
	c_[3] = other.c_[3];
    return *this;
  }

  bool operator ==(const Colour &other)
  {
    return (c_[0] == other.c_[0] && c_[1] == other.c_[1] && c_[2] == other.c_[2] && c_[3] == other.c_[3]);
  }

  bool operator !=(const Colour &other)
  {
    return !(c_[0] == other.c_[0] && c_[1] == other.c_[1] && c_[2] == other.c_[2] && c_[3] == other.c_[3]);
  }

  double R() const 
  { 
    return c_[0];
  }
  double G() const 
  { 
    return c_[1];
  }
  double B() const 
  { 
    return c_[2];
  }
  double A() const
  {
	return c_[3];
  }

  const double *begin() const
  {
    return (double*)c_;
  }

private:
	double c_[4];
};

inline Colour operator *(double s, const Colour& a)
{
  return Colour(s*a.R(), s*a.G(), s*a.B(), s*a.A());
}

inline Colour operator /(const Colour& a, double s)
{
  return Colour(a.R()/s, a.G()/s, a.B()/s, a.A()/s);
}

inline Colour operator *(const Colour& a, const Colour& b)
{
  return Colour(a.R()*b.R(), a.G()*b.G(), a.B()*b.B(), a.A()*b.A());
}

inline Colour operator +(const Colour& a, const Colour& b)
{
  return Colour(a.R()+b.R(), a.G()+b.G(), a.B()+b.B(), a.A()*b.A());
}

inline std::ostream& operator <<(std::ostream& os, const Colour& c)
{
  return os << "c<" << c.R() << "," << c.G() << "," << c.B() << "," << c.A() << ">";
}

#endif
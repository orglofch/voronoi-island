#ifndef _BIT_MAP_HPP_
#define _BIT_MAP_HPP_

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <algorithm>
#include <noise\noise.h>

#include "noiseutils.hpp"
#include "algebra.hpp"
#include "colour.hpp"

class BitMap
{
public:
	BitMap(const Vector2 &size) : 
		m_size(size), m_texture(0), m_min(-1), m_max(1) {}

	void GenerateNoiseMap(const int seed = rand());

	void SetRange(const double min, const double max);

	void GenerateTexture(const Colour &colour);

	void ApplyRadialGradient(void);

	void Render(void) const;

	int width() const { return (int)m_size[0]; }
	int height() const { return (int)m_size[1]; }

	double value(const Point2 &v) const { return value((int)v.x, (int)v.y); }
	double value(const int x, const int y) const { return m_map.GetValue(x, y); }
	void SetValue(const int x, const int y, const double val);

	GLuint texture() const { return m_texture; };
	bool HasTexture() const { return m_texture != 0; }

private:
	Vector2 m_size;

	GLuint m_texture;

	double m_min;
	double m_max;

	noise::utils::NoiseMap m_map;
};

inline BitMap operator*(const BitMap &m1, const BitMap &m2) {
	int min_width = std::min(m1.width(), m2.width());
	int min_height = std::min(m1.height(), m2.height());

	BitMap new_map(Vector2(min_width, min_height));

	for (int x = 0; x <= min_width; ++x) {
		for (int y = 0; y <= min_height; ++y) {
			new_map.SetValue(x, y, m2.value(x, y) * m1.value(x, y));
		}
	}
}

#endif
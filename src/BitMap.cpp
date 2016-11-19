#include "BitMap.hpp"

#include <iostream>

using namespace std;

void BitMap::GenerateNoiseMap(const int seed) 
{
	noise::module::Perlin myModule;

	myModule.SetSeed(seed);
	noise::utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(myModule);
	heightMapBuilder.SetDestNoiseMap(m_map);
	heightMapBuilder.SetDestSize((int)m_size[0] + 1, (int)m_size[1] + 1);
	heightMapBuilder.SetBounds(0.0, 5.0, 0.0, 5.0);
	heightMapBuilder.Build();
}

void BitMap::SetRange(const double min, const double max)
{
	// TODO uber slow when called after applyRadialGradient
	m_min = min;
	m_max = max;

	double diff = max - min;

	for (int x = 0; x <= width(); ++x) {
		for (int y = 0; y <= height(); ++y) {
			// [-1, 1] -> [0, 1]
			double val = (value(x, y) + 1.0f) / 2.0f;
			// [0, 1] -> [0, max - min]
			val *= diff;
			// [0, max - min] -> [min, max]
			val += min;

			SetValue(x, y, val);
		}
	}
}

void BitMap::ApplyRadialGradient(void)
{
	double aspect = m_size.x / m_size.y;
	Point3 center(m_size.x/2, m_size.y/2, 0);
	float primary = std::max(m_size.x/2, m_size.y/2);
	for (int x = 0; x <= width(); ++x) {
		for (int y = 0; y <= height(); ++y) {
			double val = value(x, y);
	
			Point3 position(x, y, 0);	
			if (m_size.x > m_size[1]) {
				position.y = (position.y - center.y) * aspect + center.y;
			} else {
				position.x = (position.x - center.x) * aspect + center.x;
			}

			// TODO make sure there aren't rounding errors
			float offset = (position - center).length();
			float radial_gradient =  pow(std::min(1.0f, std::max(0.0f, 1.0f - offset / primary)), 0.4);
			val *= radial_gradient;

			SetValue(x, y, val);
		}
	}
}

void BitMap::Render(void) const
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((GLfloat)m_size[0], 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((GLfloat)m_size[0], (GLfloat)m_size[1], 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, (GLfloat)m_size[1], 0.0f);
	glEnd();
}

void BitMap::GenerateTexture(const Colour &colour)
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	GLubyte* data = (GLubyte *) malloc((size_t)m_size[0] * (size_t)m_size[1] * 4 * sizeof(GLubyte));
	for (int x = 0; x < m_size[0]; ++x) {
		for (int y = 0; y < m_size[1]; ++y) { 
			int i = (int)(y * m_size[0] + x) * 4;
			float val = value(x, y);
			data[i] = (val * colour.R());
			data[i + 1] = (val * colour.G());
			data[i + 2] = (val * colour.B());
			data[i + 3] = 1.0f;
		}
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)m_size[0], (GLsizei)m_size[1], 0, GL_RGBA,
	  GL_UNSIGNED_BYTE, data);
	delete [] data;
}

void BitMap::SetValue(const int x, const int y, const double val) 
{ 
	m_map.SetValue(x, y, (float)std::max(m_min, std::min(m_max, val))); 
}
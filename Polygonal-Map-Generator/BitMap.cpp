#include "BitMap.hpp"

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
	double aspect = m_size[0] / m_size[1];
	Point3D center(m_size[0]/2, m_size[1]/2, 0);
	double primary = std::max(m_size[0]/2, m_size[1]/2);
	for (int x = 0; x <= width(); ++x) {
		for (int y = 0; y <= height(); ++y) {
			double val = value(x, y);
	
			Point3D position(x, y, 0);	
			if (m_size[0] > m_size[1]) {
				position[1] = (position[1] - center[1]) * aspect + center[1];
			} else {
				position[0] = (position[0] - center[0]) * aspect + center[0];
			}

			// TODO make sure there aren't rounding errors
			double offset = (position - center).length();
			double radial_gradient = 
				pow(std::min(1.0, std::max(0.0, (primary - offset) / primary)), 0.4);
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
			int i = (y * m_size[0] + x) * 4;
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
#ifndef _BIOME_HPP_
#define _BIOME_HPP_

#include "Colour.hpp"

class Biome 
{
public:
	static Biome *GetBiome(const double moisture, const double elevation);

	double weight() const { return m_travel_weight; };
	const Colour &colour() const { return m_colour; };

	static Biome NONE;
	static Biome OCEAN;
	static Biome LAKE;
	static Biome STEEP_BARE_ROCK;
	static Biome ICE_LAKE;
	static Biome SNOW;
	static Biome TUNDRA;
	static Biome BARE;
	static Biome SCORCHED;
	static Biome TAIGA;
	static Biome SHRUBLAND;
	static Biome TEMPERATE_DESERT;
	static Biome TEMPERATE_RAIN_FOREST;
	static Biome TEMPERATE_DECIDUOUS_FOREST;
	static Biome GRASSLAND;
	static Biome TROPICAL_RAIN_FOREST;
	static Biome TROPICAL_SEASONAL_FOREST;
	static Biome SUBTROPICAL_DESERT;
	static Biome BEACH;

private:
	Biome(const double weight, const Colour &colour) :
		m_travel_weight(weight), m_colour(colour) {}

	double m_travel_weight;
	double m_moisture;
	double m_elevation;
	double m_temperature;

	Colour m_colour;
};

#endif
#include "Biome.hpp"

#include <boost/math/special_functions/round.hpp>
#include <limits>

using boost::math::iround;

Biome Biome::NONE(1000, Colour(1.0, 0.0, 0.0));
Biome Biome::OCEAN(20, Colour(0.83, 0.75, 0.71));
Biome Biome::LAKE(25, Colour(0.93, 0.85, 0.81));
Biome Biome::STEEP_BARE_ROCK(1000, Colour(0.40, 0.35, 0.28));
Biome Biome::ICE_LAKE(80, Colour(0.75, 0.75, 0.90));
Biome Biome::SNOW(100, Colour(0.90, 0.90, 0.90));
Biome Biome::TUNDRA(80, Colour(0.80, 0.80, 0.70));
Biome Biome::BARE(50, Colour(0.73, 0.73, 0.73));
Biome Biome::SCORCHED(80, Colour(0.60, 0.60, 0.60));
Biome Biome::TAIGA(45, Colour(0.00, 0.40, 0.00));
Biome Biome::SHRUBLAND(35, Colour(0.74, 0.70, 0.37));
Biome Biome::TEMPERATE_DESERT(70, Colour(1.00, 0.74, 0.25));
Biome Biome::TEMPERATE_RAIN_FOREST(40, Colour(0.33, 0.48, 0.04));
Biome Biome::TEMPERATE_DECIDUOUS_FOREST(35, Colour(0.13, 0.30, 0.01));
Biome Biome::GRASSLAND(35, Colour(0.71, 0.67, 0.35));
Biome Biome::TROPICAL_RAIN_FOREST(55, Colour(0.38, 0.55, 0.09));
Biome Biome::TROPICAL_SEASONAL_FOREST(45, Colour(0.15, 0.42, 0.18));
Biome Biome::SUBTROPICAL_DESERT(75, Colour(0.70, 0.65, 0.43));
Biome Biome::BEACH(60, Colour(0.83, 0.75, 0.61));

Biome *Biome::GetBiome(const double moisture, const double elevation) 
{
	const unsigned int moisture_zone = iround(moisture * 5);
	const unsigned int elevation_zone = iround(elevation * 3);

	if (elevation_zone == 0) {
		if (moisture_zone == 0) {
			return &SUBTROPICAL_DESERT;
		} else if (moisture_zone == 1) {
			return &GRASSLAND;
		} else if (moisture_zone == 2 || moisture_zone == 3) {
			return &TROPICAL_SEASONAL_FOREST;
		} else {
			return &TROPICAL_RAIN_FOREST;
		}
	} else if (elevation_zone == 1) {
		if (moisture_zone == 0) {
			return &TEMPERATE_DESERT;
		} else if (moisture_zone == 1 || moisture_zone == 2) {
			return &GRASSLAND;
		} else if (moisture_zone == 3 || moisture_zone == 4) {
			return &TEMPERATE_DECIDUOUS_FOREST;
		} else {
			return &TEMPERATE_RAIN_FOREST;
		}
	} else if (elevation_zone == 2) {
		if (moisture_zone == 0 || moisture_zone == 1) {
			return &TEMPERATE_DESERT;
		} else if (moisture_zone == 2 || moisture_zone == 3) {
			return &SHRUBLAND;
		} else {
			return &TAIGA;
		}
	} else if (elevation_zone == 3) {
		if (moisture_zone == 0) {
			return &SCORCHED;
		} else if (moisture_zone == 1) {
			return &BARE;
		} else if (moisture_zone == 2) {
			return &TUNDRA;
		} else {
			return &SNOW;
		}
	} else {
		return &NONE;
	}
}
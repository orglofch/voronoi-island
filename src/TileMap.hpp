#ifndef _TILE_MAP_HPP_
#define _TILE_MAP_HPP_

#include <unordered_map>
#include <string>
#include <vector>

#include "algebra.hpp"
#include "bitmap.hpp"
#include "biome.hpp"

class Corner;
class Edge;
class Tile;

class Tile
{
public:
	Tile(const double x, const double y) :
		m_id(last_id++),
		m_biome(&Biome::NONE),
		m_center(x, y, 0),
		m_is_water(false),
		m_is_ocean(false),
		m_is_coast(false),
		m_is_border(false) {}

	int id() const { return m_id; }

	Biome *biome() { return m_biome; }
	const Biome *biome() const { return m_biome; }

	Point3 &center() { return m_center; }
	const Point3 &center() const { return m_center; }

	Vector3 &normal() { return m_normal; }
	const Vector3 &normal() const { return m_normal; }

	bool is_water() const { return m_is_water; }
	bool is_ocean() const { return m_is_ocean; }
	bool is_coast() const { return m_is_coast; }
	bool is_border() const{ return m_is_border; }

	double moisture() const { return m_moisture; }

	std::vector<Tile*> &neighbours() { return m_neighbours; }
	const std::vector<Tile*> &neighbours() const { return m_neighbours; }
	std::vector<Edge*> &edges() { return m_edges; }
	const std::vector<Edge*> &edges() const { return m_edges; }
	std::vector<Corner*> &corners() { return m_corners; }
	const std::vector<Corner*> &corners() const { return m_corners; }

	void SetBiome(Biome *biome) { m_biome = biome; }

	void SetIsWater(bool is_water) { m_is_water = is_water; }
	void SetIsOcean(bool is_ocean) { m_is_ocean = is_ocean; }
	void SetIsCoast(bool is_coast) { m_is_coast = is_coast; }
	void SetIsBorder(bool is_border) { m_is_border = is_border; }

	void SetMoisture(double moisture) { m_moisture = moisture; }

private:
	int m_id;

	Biome *m_biome;

	Point3 m_center;

	Vector3 m_normal;

	bool m_is_water;
	bool m_is_ocean;
	bool m_is_coast;
	bool m_is_border;

	double m_moisture;

	std::vector<Tile*> m_neighbours;
	std::vector<Edge*> m_edges;
	std::vector<Corner*> m_corners;

	static int last_id;
};

class Corner
{
public:
	Corner(const int x, const int y) :
		m_id(last_id++),
		m_position(x, y, 0),
		m_is_water(false),
		m_is_ocean(false),
		m_is_coast(false),
		m_is_border(false),
		m_is_river(false) {}

	int id() const { return m_id; }

	Point3 &position() { return m_position; }
	const Point3 &position() const { return m_position; }

	Vector3 &normal() { return m_normal; }
	const Vector3 &normal() const { return m_normal; }

	bool is_water() const { return m_is_water; }
	bool is_ocean() const { return m_is_ocean; }
	bool is_coast() const { return m_is_coast; }
	bool is_border() const { return m_is_border; }
	bool is_river() const { return m_is_river; }

	double moisture() const { return m_moisture; }

	std::vector<Tile*> &touches() { return m_touches; }
	const std::vector<Tile*> &touches() const { return m_touches; }
	std::vector<Edge*> &protrudes() { return m_protrudes; }
	const std::vector<Edge*> &protrudes() const { return m_protrudes; }
	std::vector<Corner*> &adjacent() { return m_adjacent; }
	const std::vector<Corner*> &adjacent() const { return m_adjacent; }

	void SetIsWater(bool is_water) { m_is_water = is_water; }
	void SetIsOcean(bool is_ocean) { m_is_ocean = is_ocean; }
	void SetIsCoast(bool is_coast) { m_is_coast = is_coast; }
	void SetIsBorder(bool is_border) { m_is_border = is_border; }
	void SetIsRiver(bool is_river) { m_is_river = is_river; }

	void SetMoisture(double moisture) { m_moisture = moisture; }

private:
	int m_id;

	Point3 m_position;

	Vector3 m_normal;

	bool m_is_water;
	bool m_is_ocean;
	bool m_is_coast;
	bool m_is_border;
	bool m_is_river;

	double m_moisture;

	std::vector<Tile*> m_touches;
	std::vector<Edge*> m_protrudes;
	std::vector<Corner*> m_adjacent;

	static int last_id;
};

class Edge
{
public:
	Edge() :
		m_id(last_id++),
		m_is_coast(false),
		m_is_river(false),
		m_is_road(false),
		m_d_edge(NULL, NULL),
		m_v_edge(NULL, NULL) {}

	int id() const { return m_id; }

	bool is_coast() const { return m_is_coast; }
	bool is_river() const { return m_is_river; }
	bool is_road() const { return m_is_road; }

	std::pair<Tile*, Tile*> &d_edge() { return m_d_edge; }
	const std::pair<Tile*, Tile*> &d_edge() const { return m_d_edge; }

	std::pair<Corner*, Corner*> &v_edge() { return m_v_edge; }
	const std::pair<Corner*, Corner*> &v_edge() const { return m_v_edge; }

	bool HasDEdge() const { return m_d_edge.first != NULL && m_d_edge.second != NULL; }

	// TODO precompute???
	Point3 d_center() const { 
		if (m_d_edge.first != NULL && m_d_edge.second != NULL) {
			return (m_d_edge.first->center() + m_d_edge.second->center()) / 2;
		}
		return Point3();
	}
	Point3 v_center() const { 
		if (m_v_edge.first != NULL && m_v_edge.second != NULL) {
			return (m_v_edge.first->position() + m_v_edge.second->position()) / 2;
		}
		return Point3();
	}
	double v_length() const {
		if (m_d_edge.first != NULL && m_d_edge.second != NULL) {
			return (m_d_edge.first->center() - m_d_edge.second->center()).length();
		}
		return 0;
	}
	double d_length() const {
		if (m_v_edge.first != NULL && m_v_edge.second != NULL) {
			return (m_v_edge.first->position() - m_v_edge.second->position()).length();
		}
		return 0;
	}

	void SetIsCoast(bool is_coast) { m_is_coast = is_coast; }
	void SetIsRiver(bool is_river) { m_is_river = is_river; }
	void SetIsRoad(bool is_road) { m_is_road = is_road; }

	void SetDEdge(Tile *t1, Tile *t2) { m_d_edge = std::make_pair(t1, t2); }
	void SetVEdge(Corner *c1, Corner *c2) { m_v_edge = std::make_pair(c1, c2); }

	Tile* OtherTile(const Tile* tile) {
		return m_d_edge.first == tile ? m_d_edge.second : m_d_edge.first;
	}
	const Tile* OtherTile(const Tile *tile) const {
		return m_d_edge.first == tile ? m_d_edge.second : m_d_edge.first;
	}

	Corner* OtherCorner(const Corner *corner) {
		return m_v_edge.first == corner ? m_v_edge.second : m_v_edge.first;
	}
	const Corner* OtherCorner(const Corner *corner) const { 
		return m_v_edge.first == corner ? m_v_edge.second : m_v_edge.first;
	}

private:
	int m_id;

	bool m_is_coast;
	bool m_is_river;
	bool m_is_road;

	std::pair<Tile*, Tile*> m_d_edge;
	std::pair<Corner*, Corner*> m_v_edge;

	static int last_id;
};

class Location
{
public:
	Location(Tile *tile) :
		m_id(last_id++),
		m_tile(tile) {}

	int id() { return m_id; }

	Tile *tile() { return m_tile; }
	const Tile *tile() const { return m_tile; }

private:
	int m_id;

	Tile *m_tile;

	static int last_id;
};

class Road
{
public:
	Road(Location *l1, Location *l2) :
		m_id(last_id++),
		m_locations(l1, l2) {}

	int id() { return m_id; }

	std::pair<Location*, Location*> locations() { return m_locations; }
	const std::pair<Location*, Location*> locations() const { return m_locations; }

	std::vector<Point3> &points() { return m_points; }
	const std::vector<Point3> &points() const { return m_points; }

private:
	int m_id;

	std::pair<Location*, Location*> m_locations;

	std::vector<Point3> m_points;

	static int last_id;
};

class River
{
public:
	River() : m_id(last_id++) {}

	int id() { return m_id; }

	std::vector<Corner*> &corners() { return m_corners; }
	const std::vector<Corner*> &corners() const { return m_corners; }

private:
	int m_id;

	std::vector<Corner*> m_corners;
	static int last_id;
};

struct Chunk
{

};

class TileMap
{
public:
	TileMap(const Vector2 &size, const int seed = rand());
	virtual ~TileMap(void);

	void Generate(void);

	void RenderMap(void);
	void RenderElevationMap(void) const;
	void RenderMoistureMap(void) const;
	void RenderBiomeMap(void) const;

	void SetSeed(const int seed);

	void Step(const double deltaTime);

	float width() { return m_size.x; };
	float height() { return m_size.y; };
	
protected:
	enum Style {
		STYLE_UNDEFINED,
		STYLE_MESH,
		STYLE_HEIGHT,
		STYLE_MOISTURE,
		STYLE_BIOME,
		STYLE_FULL,
	};

	enum State {
		STATE_UNDEFINED,
		STATE_START,
		STATE_GENERATE_TILES,
		STATE_ASSIGN_BORDERS,
		STATE_ASSIGN_ELEVATION,
		STATE_ASSIGN_NORMALS,
		STATE_ASSIGN_WATER,
		STATE_ASSIGN_OCEAN,
		STATE_ASSIGN_COAST,
		STATE_GENERATE_RIVERS,
		STATE_ASSIGN_MOISTURE,
		STATE_ASSIGN_BIOMES,
		STATE_GENERATE_LOCATIONS,
		STATE_GENERATE_ROADS,
		STATE_DONE,
	};
	void UpdateState(State state);
	void PerformActionForState(State state);
	static std::string StateToString(State state);
	static Style StyleForState(State state);

	void RenderMap(Style style);

	void BuildVBO(void);

	// Drawing operations
	void DrawTile(const Tile *tile, const Style style) const;
	void DrawCoast(void) const;
	void DrawRivers(void) const;
	void DrawLocations(void) const;
	void DrawRoads(void) const;
	void DrawWater(void) const;

	// Generation operations
	virtual void GenerateTiles(void) = 0;
	void GenerateRivers(void);
	void GenerateLocations(void);
	void GenerateRoads(void);
	void GenerateRoad(Location *l1, Location *l2);

	double TileToTileWeight(const Tile *t1, const Tile *t2, const Edge *via) const;
	double TileToEdgeWeight(const Tile *source, const Edge *edge) const;

	// Assignment operations
	void AssignElevation(void);
	void AssignNormals(void);
	void AssignBorders(void);
	void AssignWater(void);
	void AssignCoast(void);
	void AssignOcean(void);
	void AssignMoisture(void);
	void AssignBiomes(void);
	
	State m_state;

	Matrix4x4 m_transform;

	// Maps
	BitMap m_elevation_map;
	BitMap m_moisture_map;

	// Size member
	Vector2 m_size;
	int m_columns;
	int m_rows;

	int m_seed;

	double m_heightVariation;
	double m_watermark;

	// Graph objects
	std::vector<Tile*> m_tiles;
	std::vector<Edge*> m_edges;
	std::vector<Corner*> m_corners;
	std::vector<Location*> m_locations;
	std::vector<Road*> m_roads;
	std::vector<River*> m_rivers;

	// Quick Graph reference lists
	std::vector<Tile*> m_tiles_land;
	std::vector<Tile*> m_tiles_water;
	std::vector<Edge*> m_edges_coast;
	std::vector<Corner*> m_corners_land;
	std::vector<Corner*> m_corners_water;
	std::vector<Point3> m_moisture_providers;
};

class VoronoiTileMap : public TileMap
{
public:
	VoronoiTileMap(const Vector2 &size, const int sites) : 
		TileMap(size),
		m_sites(sites) {}

private:
	virtual void GenerateTiles(void) override;

	int m_sites;
};

class HexTileMap : public TileMap 
{
public:
	HexTileMap(const Vector2 &size) : 
		TileMap(size) {}

private:
	virtual void GenerateTiles(void) override;
};

class GridTileMap : public TileMap
{
public:
	GridTileMap(const Vector2 &size, const int cellSize) : 
		TileMap(size), 
		m_cellSize(cellSize) {}

private:
	virtual void GenerateTiles(void) override;

	int m_cellSize;
};

#endif
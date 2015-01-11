#include "TileMap.hpp"

#include <boost\polygon\voronoi.hpp>
#include <boost\timer.hpp>
#include <queue> 

#include "OpenGL.hpp"

#include "WindowManager.hpp"
#include "ShaderManager.hpp"
#include "Assertion.hpp"
#include "KDTree.hpp"

using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;
using boost::polygon::point_data;
using boost::timer;
using std::string;
using std::vector;
using std::queue;
using sf::Shader;
using std::list;

int Corner::last_id = 0;
int Edge::last_id = 0;
int Tile::last_id = 0;
int Location::last_id = 0;
int Road::last_id = 0;
int River::last_id = 0;

TileMap::TileMap(const Vector2D &size, const int seed) : 
	m_state(STATE_START),
	m_size(size), 
	m_seed(seed),
	m_watermark(0.32),
	m_sun(Point3D(1000, 0, 0)),
	m_heightVariation(200),		
	m_elevation_map(size),
	m_moisture_map(size) 
{
	srand(m_seed);

	//m_depth_texture.create(m_size[0], m_size[1], true);
}

TileMap::~TileMap()
{
	for (Corner *corner : m_corners) {
		delete corner;
	}
	for (Edge *edge : m_edges) {
		delete edge;
	}
	for (Tile *tile : m_tiles) {
		delete tile;
	}
	for (Location *location : m_locations) {
		delete location;
	}
	for (River *river : m_rivers) {
		delete river;
	}
	for (Road *road : m_roads) {
		delete road;
	}
}

void TileMap::RenderMap(void)
{
	RenderMap(StyleForState(m_state));
}

void TileMap::RenderElevationMap(void) const
{
	glPushMatrix();
		glMultMatrixd(m_transform.transpose().begin());
		m_elevation_map.Render();
	glPopMatrix();
}

void TileMap::RenderMoistureMap(void) const
{
	glPushMatrix();
		glMultMatrixd(m_transform.transpose().begin());
		m_moisture_map.Render();
	glPopMatrix();
}

void TileMap::RenderBiomeMap(void) const
{
}

void TileMap::UpdateState(State state)
{
	m_state = state;
	std::cout << StateToString(state) << std::endl;
}

void TileMap::PerformActionForState(State state)
{
	switch (state) {
		case STATE_START:
			break;
		case STATE_GENERATE_TILES:
			GenerateTiles();
			break;
		case STATE_ASSIGN_BORDERS:
			AssignBorders();
			break;
		case STATE_ASSIGN_ELEVATION:
			AssignElevation();
			break;
		case STATE_ASSIGN_NORMALS:
			AssignNormals();
			break;
		case STATE_ASSIGN_WATER:
			AssignWater();
			break;
		case STATE_ASSIGN_COAST:
			AssignCoast();
			break;
		case STATE_ASSIGN_OCEAN:
			AssignOcean();
			break;
		case STATE_GENERATE_RIVERS:
			GenerateRivers();
			break;
		case STATE_ASSIGN_MOISTURE:
			AssignMoisture();
			break;
		case STATE_ASSIGN_BIOMES:
			AssignBiomes();
			break;
		case STATE_GENERATE_LOCATIONS:
			GenerateLocations();
			break;
		case STATE_GENERATE_ROADS:
			GenerateRoads();
			break;
		case STATE_DONE:
			break;
	}
}

string TileMap::StateToString(State state)
{
	switch (state)
	{
		case STATE_START: 
			return "STATE_START";
		case STATE_GENERATE_TILES: 
			return "STATE_GENERATE_TILES";
		case STATE_ASSIGN_BORDERS: 
			return "STATE_ASSIGN_BORDERS";
		case STATE_ASSIGN_ELEVATION: 
			return "STATE_ASSIGN_ELEVATION";
		case STATE_ASSIGN_NORMALS:
			return "STATE_ASSIGN_NORMALS";
		case STATE_ASSIGN_WATER: 
			return "STATE_ASSIGN_WATER";
		case STATE_ASSIGN_COAST: 
			return "STATE_ASSIGN_COAST";
		case STATE_ASSIGN_OCEAN: 
			return "STATE_ASSIGN_OCEAN";
		case STATE_GENERATE_RIVERS: 
			return "STATE_GENERATE_RIVERS";
		case STATE_ASSIGN_MOISTURE: 
			return "STATE_ASSIGN_MOISTURE";
		case STATE_ASSIGN_BIOMES: 
			return "STATE_ASSIGN_BIOMES";
		case STATE_GENERATE_LOCATIONS: 
			return "STATE_GENERATE_LOCATIONS";
		case STATE_GENERATE_ROADS: 
			return "STATE_GENERATE_ROADS";
		case STATE_DONE:
			return "STATE_DONE";
	}
	NOTREACHED();
	return "???";
}

TileMap::Style TileMap::StyleForState(State state)
{
	switch (state) 
	{
		case STATE_START:
		case STATE_GENERATE_TILES:
		case STATE_ASSIGN_BORDERS:
		case STATE_ASSIGN_ELEVATION:
			return STYLE_MESH;
		case STATE_ASSIGN_NORMALS:
		case STATE_ASSIGN_WATER:
		case STATE_ASSIGN_COAST:
		case STATE_ASSIGN_OCEAN:
		case STATE_GENERATE_RIVERS:
		case STATE_ASSIGN_MOISTURE:
			return STYLE_HEIGHT;
		case STATE_ASSIGN_BIOMES:
		case STATE_GENERATE_LOCATIONS:
		case STATE_GENERATE_ROADS:
			return STYLE_MOISTURE;
		case STATE_DONE:
			return STYLE_FULL;
	}
	NOTREACHED();
	return STYLE_UNDEFINED;
}

void TileMap::RenderMap(Style style)
{
	// Render to texture
	//m_render_texture.setActive();
	/*if (!m_render_texture.create(m_size[0], m_size[1])) {
		std::cout << "Failed to create render texture" << std::endl;
		exit(EXIT_FAILURE);
	}*/
	//m_render_texture.clear();
	//m_render_texture.setActive(true);
	sf::Shader::bind(&eShaderManager.GetShader(ShaderManager::SHADER_PASS_THROUGH));

	glPushMatrix();
	{
		glMultMatrixd(m_transform.transpose().begin());
		//glBindTexture(GL_TEXTURE_2D, 0);
		for (Tile *tile : m_tiles) {
			DrawTile(tile, style);
		}
		DrawRivers();
		DrawRoads();
		DrawLocations();
		if (style == STYLE_FULL) {
			DrawWater();
		}
	}
	glPopMatrix();
	//buffer.display();
	//buffer.setActive(false);

	// Post processing
	//m_render_texture.setActive(false);
	/*eWindowManager.window().setActive();
	glViewport(0, 0, m_size[0], m_size[1]);
	Shader &shader = eShaderManager.GetShader(ShaderManager::SHADER_TILT_SHIFT);

	shader.setParameter("texture", test);
	sf::Shader::bind(&shader);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	{
		glTexCoord2d(0.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
		glTexCoord2d(0.0, 1.0); glVertex3d(0.0, m_size[1], 0.0);
		glTexCoord2d(1.0, 1.0); glVertex3d(m_size[0], m_size[1], 0.0);
		glTexCoord2d(1.0, 0.0); glVertex3d(m_size[0], 0.0, 0.0);
	}
	glEnd();*/
}

void TileMap::DrawTile(const Tile *tile, const Style style) const
{
	glLineWidth(1);
	switch (style)
	{
		case STYLE_MESH:
			glBegin(GL_LINE_LOOP);
			{
				for (const Corner *corner : tile->corners()) {
					const Point3D &p = corner->position();
					glVertex3d(p[0], p[1], p[2] * -m_heightVariation);
				}
			}
			glEnd();
			glBegin(GL_POINTS);
			{
				const Point3D &p = tile->center();
				glVertex3d(p[0], p[1], p[2] * -m_heightVariation);
			}
			glEnd();
			break;
		case STYLE_HEIGHT:
			glColor3d(0.0, tile->center()[2], 0.0);
			glBegin(GL_POLYGON);
			{
				for (const Corner *corner : tile->corners()) {
					const Point3D &p = corner->position();
					glNormal3dv(tile->normal().begin());
					glVertex3d(p[0], p[1], p[2] * -m_heightVariation);
				}
			}
			glEnd();
			break;
		case STYLE_MOISTURE:
			glColor3d(0.0, 0.0, tile->moisture());
			glBegin(GL_POLYGON);
			{
				for (const Corner *corner : tile->corners()) {
					const Point3D &p = corner->position();
					glNormal3dv(tile->normal().begin());
					glVertex3d(p[0], p[1], p[2] * -m_heightVariation);
				}
			}
			glEnd();
			break;
		case STYLE_BIOME:
			glColor3dv(tile->biome()->colour().begin());
			glBegin(GL_POLYGON);
			{
				for (const Corner *corner : tile->corners()) {
					const Point3D &p = corner->position();
					glNormal3dv(tile->normal().begin());
					glVertex3d(p[0], p[1], p[2] * -m_heightVariation);
				}
			}
			glEnd();
			break;
		case STYLE_FULL:
			glColor3dv(tile->biome()->colour().begin());
			glBegin(GL_POLYGON);
			{
				for (const Corner *corner : tile->corners()) {
					const Point3D &p = corner->position();
					glNormal3dv(tile->normal().begin());
					glVertex3d(p[0], p[1], p[2] * -m_heightVariation);
				}
			}
			glEnd();
			break;
	}
}

void TileMap::DrawCoast(void) const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (const Edge *edge : m_edges_coast) {
		glBegin(GL_LINES);
		{
			glVertex3dv(edge->v_edge().first->position().begin());
			glVertex3dv(edge->v_edge().second->position().begin());
		}
		glEnd();
	}
}

void TileMap::DrawRivers(void) const
{
	glLineWidth(2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(0.0f, 0.0f, 1.0f);
	for (const River *river : m_rivers) {
		glBegin(GL_LINE_STRIP);
		{
			for (const Corner *corner : river->corners()) {
				const Point3D &p = corner->position();
				glVertex3d(p[0], p[1], p[2] * -m_heightVariation - 1);
			}
		}
		glEnd();
	}
}

void TileMap::DrawLocations(void) const
{
	glLineWidth(5);
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (Location *location : m_locations) {
		glBegin(GL_LINES);
		{
			const Point3D &p = location->tile()->center();
			glVertex3d(p[0], p[1], p[2] * -m_heightVariation - 1);
			glVertex3d(p[0], p[1], p[2] * -m_heightVariation - 15);
		}
		glEnd();
	}
}

void TileMap::DrawRoads(void) const
{
	glLineWidth(2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(0.0f, 0.0f, 0.0f);
	for (Road *road : m_roads) {
		glBegin(GL_LINE_STRIP);
		{
			for (const Point3D &p : road->points()) {
				glVertex3d(p[0], p[1], p[2] * -m_heightVariation - 3);
			}
		}
		glEnd();
	}
}

void TileMap::DrawWater(void) const
{
	GLdouble vertex[8][3] = {
		{ 0.0, 0.0, 0.0 },
		{ m_size[0], 0.0, 0.0 },
		{ m_size[0], m_size[1], 0.0 },
		{ 0.0, m_size[1], 0.0 },
		{ 0.0, 0.0, m_watermark * -m_heightVariation },
		{ m_size[0], 0.0, m_watermark * -m_heightVariation },
		{ m_size[0], m_size[1], m_watermark * -m_heightVariation },
		{ 0.0, m_size[1], m_watermark * -m_heightVariation },
	};

	glBindTexture(GL_TEXTURE_2D, 0);
	glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
	glBegin(GL_QUADS);
	{
		glVertex3dv(vertex[0]);
		glVertex3dv(vertex[3]);
		glVertex3dv(vertex[2]);
		glVertex3dv(vertex[1]);

		glVertex3dv(vertex[4]);
		glVertex3dv(vertex[5]);
		glVertex3dv(vertex[6]);
		glVertex3dv(vertex[7]);

		glVertex3dv(vertex[0]);
		glVertex3dv(vertex[1]);
		glVertex3dv(vertex[5]);
		glVertex3dv(vertex[4]);

		glVertex3dv(vertex[1]);
		glVertex3dv(vertex[2]);
		glVertex3dv(vertex[6]);
		glVertex3dv(vertex[5]);

		glVertex3dv(vertex[3]);
		glVertex3dv(vertex[7]);
		glVertex3dv(vertex[6]);
		glVertex3dv(vertex[2]);

		glVertex3dv(vertex[0]);
		glVertex3dv(vertex[4]);
		glVertex3dv(vertex[7]);
		glVertex3dv(vertex[3]);
	}
	glEnd();
}

void TileMap::SetSeed(const int seed) 
{
	m_seed = seed;
	srand(m_seed);
}

void TileMap::Step(const double deltaTime)
{
	PerformActionForState(m_state);
	m_transform = rotation('x', 0.05) * m_transform;

	m_sun.Step(deltaTime);
}

void TileMap::Generate(void) 
{	
	UpdateState(STATE_GENERATE_TILES);
}

void TileMap::GenerateRivers(void)
{
	// TODO make this check smarter
	if (m_corners_land.size() == 0) {
		return;
	}
	// TODO make number random
	for (int i = 0; i < 50; ++i) {
		River *river = new River();

		// Choose a random starting point
		Corner *source = m_corners_land.at(rand() % m_corners_land.size());

		river->corners().push_back(source);
		m_moisture_providers.push_back(source->position());

		while (!source->is_water() && !source->is_river()) {
			source->SetIsRiver(true);

			int min_corner_index = -1;
			int min_edge_index = -1;
			double min_height = std::numeric_limits<double>::infinity();
			// Find the lowest corner for water to travel
			for (const Edge *edge : source->protrudes()) {
				// Skip the previous edge, TODO can probably make smarter
				if (find(river->corners().begin(), river->corners().end(), edge->OtherCorner(source)) == river->corners().end()) {
					const Corner *corner = edge->OtherCorner(source);
					if (corner->position()[2] < min_height) {
						min_corner_index = corner->id();
						min_edge_index = edge->id();
						min_height = corner->position()[2];
					}
				}
			}

			// TODO make this check better
			if (min_edge_index == -1) {
				break;
			}
			Edge *next_edge = m_edges.at(min_edge_index);
			Corner *next_corner = m_corners.at(min_corner_index);

			// Erode to slightly below source height
			if (next_corner->position()[2] > source->position()[2]) {
				next_corner->position()[2] = std::max(m_watermark, source->position()[2] - 0.001);
			}
			next_edge->SetIsRiver(true);

			m_moisture_providers.push_back(next_corner->position());
			river->corners().push_back(next_corner);

			source = next_corner;
		}
		m_rivers.push_back(river);
	}

	UpdateState(STATE_ASSIGN_MOISTURE);
}

void TileMap::GenerateLocations(void)
{
	// TODO make number of locations based on size of landmass
	// TODO intelligently place locations
	for (int i = 0; i < 5; ++i) {
		Tile *tile = m_tiles_land.at(rand() % m_tiles_land.size());
		Location *location = new Location(tile);

		m_locations.push_back(location);
	}

	UpdateState(STATE_GENERATE_ROADS);
}

void TileMap::GenerateRoads(void)
{
	/*for (size_t i = 0; i < m_locations.size(); ++i) {
		Location *l1 = m_locations.at(i);
		for (size_t j = i + 1; j < m_locations.size(); ++j) {
			Location *l2 = m_locations.at(j);
			GenerateRoad(l1, l2);
		}
	}*/

	double *dist = new double[m_tiles.size()];
	int *prev = new int[m_tiles.size()];

	// Dijkstras to find shortest path from each location to
	// each other location
	for (size_t i = 0; i < m_locations.size(); ++i) {
		Location *location = m_locations.at(i);
		list<int> Q;
		for (size_t j = 0; j < m_tiles.size(); ++j) {
			dist[j] = std::numeric_limits<double>::infinity();
			prev[j] = -1;
			Q.push_back(j);
		}

		const Tile *source = location->tile();
		dist[source->id()] = 0;
		prev[source->id()] = -1;

		// Generate dist and prev graphs
		while (!Q.empty())
		{
			// Find lowest cost tile to travel too
			double min_dist = std::numeric_limits<double>::infinity();
			int min_tile_index = -1;
			for (int index : Q) {
				if (dist[index] <= min_dist) {
					min_dist = dist[index];
					min_tile_index = index;
				}
			}
			Q.remove(min_tile_index);

			// Try to update neighbour distances
			Tile *next_tile = m_tiles.at(min_tile_index);
			for (const Edge *edge : next_tile->edges()) {
				if (edge->HasDEdge()) {
					double new_dist = dist[min_tile_index] + TileToTileWeight(source, next_tile, edge);
					const Tile *neighbour = edge->OtherTile(next_tile);
					if (new_dist < dist[neighbour->id()]) {
						dist[neighbour->id()] = new_dist;
						prev[neighbour->id()] = edge->id();
					}
				}
			}
		}

		// Create roads based on dist and prev between all location pairs
		for (size_t j = i + 1; j < m_locations.size(); ++j) {
			Location *other_location = m_locations.at(j);
			Road *road = new Road(location, other_location);

			road->points().push_back(other_location->tile()->center());

			const Tile *prev_tile = other_location->tile();
			int current_edge_index = prev[other_location->tile()->id()];
			while (current_edge_index != -1)
			{
				Edge *current_edge = m_edges.at(current_edge_index);
				const Tile *next_tile = current_edge->OtherTile(prev_tile);
				road->points().push_back(current_edge->v_center());
				current_edge->SetIsRoad(true);

				road->points().push_back(next_tile->center());

				prev_tile = next_tile;
				current_edge_index = prev[next_tile->id()];
			} 
			m_roads.push_back(road);
		}
	}

	delete dist;
	delete prev;

	UpdateState(STATE_DONE);
}

void TileMap::GenerateRoad(Location *l1, Location *l2)
{
	// A* search
	const Tile *start = l1->tile();
	const Tile *end = l2->tile();

	list<const Tile*> closed;
	list<const Tile*> open;
	open.push_back(start);
	int *came_from = new int[m_tiles.size()];  
	double *g_score = new double[m_tiles.size()];
	double *f_score = new double[m_tiles.size()];
	for (size_t i = 0; i < m_tiles.size(); ++i) {
		came_from[i] = -1;
		g_score[i] = std::numeric_limits<double>::infinity();
		f_score[i] = std::numeric_limits<double>::infinity();
	}

	g_score[start->id()] = 0;   
	f_score[start->id()] = g_score[start->id()] + 
		std::max(start->center()[0] - end->center()[0], start->center()[1] - end->center()[1]);// (start->center() - end->center()).length();

	while (!open.empty()) {
		double min_f_score = std::numeric_limits<double>::infinity();
		const Tile *current;
		for (const Tile *tile : open) {
			if (f_score[tile->id()] < min_f_score) {
				current = tile;
				min_f_score = f_score[tile->id()];
			}
		}
		if (current->id() == end->id()) {
			break;
		}

		open.remove(current);
		closed.push_back(current);
		for (const Edge *edge : current->edges()) {
			if (edge->HasDEdge()) {
				const Tile *neighbour = edge->OtherTile(current);
				if (std::find(closed.begin(), closed.end(), neighbour) != closed.end()) {
					continue;
				}
				double tentative_g_score = 
					g_score[current->id()] + (current->center() - neighbour->center()).length();

				if (std::find(open.begin(), open.end(), neighbour) == open.end() ||
					tentative_g_score < g_score[neighbour->id()]) {
					came_from[neighbour->id()] = edge->id();
					g_score[neighbour->id()] = tentative_g_score;
					f_score[neighbour->id()] = 
						g_score[neighbour->id()] + 
						std::max(neighbour->center()[0] - end->center()[0], neighbour->center()[1] - end->center()[1]);// (neighbour->center() - end->center()).length();
					if (std::find(open.begin(), open.end(), neighbour) == open.end()) {
						open.push_back(neighbour);
					}
				}
			}
		}
	}

	Road *road = new Road(l1, l2);
	road->points().push_back(end->center());

	const Tile *current = end;
	while (current->id() != start->id()) {
		Edge *current_edge = m_edges.at(came_from[current->id()]);
		const Tile *next_tile = current_edge->OtherTile(current);

		current_edge->SetIsRoad(true);
		road->points().push_back(current_edge->v_center());
		road->points().push_back(next_tile->center());

		current = next_tile;
	}
	m_roads.push_back(road);

	delete[] came_from;
	delete[] g_score;
	delete[] f_score;
}

double TileMap::TileToTileWeight(const Tile *t1, const Tile *t2, const Edge *via) const
{
	// Embarking and disembarking from/to water is 5x harder 
	// TODO possibly remove in favour of only moving via port cities
	double water_land_transition_weight =
		(t1->is_water() && !t2->is_water()) ||
		(!t1->is_water() && t2->is_water()) ? 5.0 : 1.0;

	return TileToEdgeWeight(t1, via) + TileToEdgeWeight(t2, via);
}

double TileMap::TileToEdgeWeight(const Tile *tile, const Edge *edge) const
{
	/* TODO static_assert(edge->d_edge.first == source->index ||
	edge->d_edge.second == source->index); */

	Vector3D delta_d = (tile->center() - edge->v_center());
	double length = delta_d.length();
	// TODO make this independant of height variation
	double slope = abs(delta_d[2] * m_heightVariation * 2) / sqrt(pow(delta_d[0], 2) + pow(delta_d[1], 2));

	// Steep terrain is harder to traverse but not for water tiles
	double slope_weight = tile->is_water() ? 1.0 : std::max(1.0, pow(slope, 4.0));
	// Certain biomes are harder to traverse
	double biome_weight = tile->biome()->weight();

	// Edges that are already roads are 4x as easy to traverse
	double road_weight = edge->is_road() ? 0.25 : 1.0;
	// Edges that are rivers are 2x as hard to traverse
	double river_weight = edge->is_river() ? 2.0 : 1.0;

	return length * biome_weight  * road_weight * river_weight * slope_weight;
}

void TileMap::AssignElevation(void)
{
	m_elevation_map.GenerateNoiseMap(m_seed);
	m_elevation_map.SetRange(0, 1);
	m_elevation_map.ApplyRadialGradient();
	m_elevation_map.GenerateTexture(Colour(1.0, 1.0, 0.0));

	for (Corner *corner : m_corners) {
		corner->position()[2] = m_elevation_map.value(corner->position()[0], corner->position()[1]);
	}

	for (Tile *tile : m_tiles) {
		tile->center()[2] = m_elevation_map.value(tile->center()[0], tile->center()[1]);
	}

	UpdateState(STATE_ASSIGN_NORMALS);
}

void TileMap::AssignNormals(void)
{
	for (Tile *tile : m_tiles) {
		Point3D p0 = tile->center();
		Point3D p1 = tile->corners().at(0)->position();
		Point3D p2 = tile->corners().at(1)->position();
		p0[2] = p0[2] * -m_heightVariation;
		p1[2] = p1[2] * -m_heightVariation;
		p2[2] = p2[2] * -m_heightVariation;
		tile->normal() = (p1 - p0).cross(p2 - p0);
		if (tile->normal()[0] == 0 && tile->normal()[1] == 0 && tile->normal()[2] == 0) {
			tile->normal() = Vector3D(0.0, 0.0, 1.0);
		}
	}

	UpdateState(STATE_ASSIGN_WATER);
}

void TileMap::AssignBorders(void) 
{
	for (Edge *edge : m_edges) {
		if (!edge->HasDEdge()) {
			edge->v_edge().first->SetIsBorder(true);
			edge->v_edge().second->SetIsBorder(true);
		}
	}
	for (Tile *tile : m_tiles) {
		for (Corner *corner : tile->corners()) {
			if (corner->is_border()) {
				tile->SetIsBorder(true);
				break;
			}
		}
	}

	UpdateState(STATE_ASSIGN_ELEVATION);
}

void TileMap::AssignWater(void)
{
	// Mark graph
	for (Corner *corner : m_corners) {
		corner->SetIsWater(corner->position()[2] < m_watermark);
	}
	for (Tile *tile : m_tiles) {
		tile->SetIsWater(true);
		for (Corner *corner : tile->corners()) {
			if (!corner->is_water()) {
				tile->SetIsWater(false);
				break;
			}
		}
	}

	// Create quick reference lists
	for (Corner *corner : m_corners) {
		if (corner->is_water()) {
			m_corners_water.push_back(corner);
		} else {
			m_corners_land.push_back(corner);
		}
	}
	for (Tile *tile : m_tiles) {
		if (tile->is_water()) {
			m_tiles_water.push_back(tile);
		} else {
			m_tiles_land.push_back(tile);
		}
	}

	UpdateState(STATE_ASSIGN_OCEAN);
}

void TileMap::AssignCoast(void)
{
	// Mark graph
	for (Edge *edge : m_edges) {
		if (edge->HasDEdge()) {
			Tile *t1 = edge->d_edge().first;
			Tile *t2 = edge->d_edge().second;

			if ((t1->is_water() && !t2->is_water()) ||
				(!t1->is_water() && t2->is_water())) {
				if (!t1->is_water()) {
					t1->SetIsCoast(true);
				}
				if (!t2->is_water()) {
					t2->SetIsCoast(true);
				}
				edge->SetIsCoast(true);
				edge->v_edge().first->SetIsCoast(true);
				edge->v_edge().second->SetIsCoast(true);

				// TODO possible extract lake marker
				if ((t1->is_water() && !t1->is_ocean()) ||
					(t2->is_water() && !t2->is_ocean())) {
					m_moisture_providers.push_back(edge->v_edge().first->position());
					m_moisture_providers.push_back(edge->v_edge().second->position());
				}
			} 
		}
	}

	// Create quick reference lists
	for (Edge *edge : m_edges) {
		if (edge->is_coast()) {
			m_edges_coast.push_back(edge);
		}
	}

	UpdateState(STATE_GENERATE_RIVERS);
}

void TileMap::AssignOcean(void) 
{
	bool *visited = new bool[m_corners.size()];
	for(unsigned int i = 0; i < m_corners.size(); ++i) {
        visited[i] = false;
	}
 
	queue<int> queue;
	queue.push(0);
	unsigned int last_free_index = 0;

	while (!queue.empty()) {
		vector<int> chain;
		bool ocean = false;
		while(!queue.empty())
		{ 
			int index = queue.front();
			queue.pop();
			if (visited[index]) {
				continue;
			}
			visited[index] = true;

			Corner *corner = m_corners.at(index);
			if (!corner->is_water()) {
				continue;
			}
			chain.push_back(index);

			corner->SetIsOcean(ocean);
			if (corner->is_border()) {
				ocean = true;
				corner->SetIsOcean(true);
				// Retroactively mark ocean in chain
				for (int index : chain) {
					m_corners.at(index)->SetIsOcean(true);
				}
			}

			for (Corner *corner : corner->adjacent()) {
				queue.push(corner->id());
			}
		}
		chain.clear();
		while (last_free_index < m_corners.size()) {
			if (!visited[last_free_index]) {
				queue.push(last_free_index);
				break;
			}
			++last_free_index;
		}
	}
	delete [] visited;

	for (Tile *tile : m_tiles) {
		for (Corner *corner : tile->corners()) {
			if (corner->is_ocean()) {
				tile->SetIsOcean(true);
				break;
			}
		}
	}

	UpdateState(STATE_ASSIGN_COAST);
}

void TileMap::AssignMoisture(void)
{
	m_moisture_map.GenerateNoiseMap(m_seed);
	m_moisture_map.SetRange(0, 1);

	KDTree moisture_tree(m_moisture_providers);

	for (Corner *corner : m_corners) {
		const Point3D &p = corner->position();
		double distance = (moisture_tree.Closest(p) - p).length();
		corner->SetMoisture(std::min(1.0, std::min(1.0, (std::min(1.0, 1 / pow(distance, 0.5) * 3.5) * (0.6)) +
			m_moisture_map.value((int)p[0], (int)p[1]) * (0.3))));
	}

	// Average corner moisture
	for (Tile *tile : m_tiles) {
		double moisture_sum = 0;
		for (const Corner *corner : tile->corners()) {
			moisture_sum += corner->moisture();
		}
		tile->SetMoisture(moisture_sum / tile->corners().size());
	}

	UpdateState(STATE_ASSIGN_BIOMES);
}

void TileMap::AssignBiomes(void)
{
	for (Tile *tile : m_tiles) {
		if (tile->is_water()) {
			if (tile->is_ocean() && tile->center()[2] < m_watermark - 0.1) {
				tile->SetBiome(&Biome::OCEAN);
			} else {
				tile->SetBiome(&Biome::LAKE);
			}
		//} else if (tile->is_coast) {
		//	tile->biome = &Biome::BEACH;
		} else {
			tile->SetBiome(Biome::GetBiome(tile->moisture(), (tile->center()[2] - m_watermark) / (1.0 - m_watermark)));
		}
	}

	UpdateState(STATE_GENERATE_LOCATIONS);
}

void TileMap::BuildVBO(void) 
{
}

void VoronoiTileMap::GenerateTiles(void)
{
	vector<point_data<double>> sites;
	for (int i = 0; i < m_sites; ++i) {
		double x = rand() % ((int)m_size[0] + 1);
		double y = rand() % ((int)m_size[1] + 1);
		point_data<double> site(x, y);

		sites.push_back(site);
	}

	voronoi_diagram<double> vd;
	construct_voronoi(sites.begin(), sites.end(), &vd);

	// Create maps of graph objects to prevent double instantiation
	std::unordered_map<const voronoi_diagram<double>::edge_type::voronoi_vertex_type *, Corner *> corner_map;
	std::unordered_map<const voronoi_diagram<double>::edge_type *, Edge *> edge_map;

	for (auto it = vd.cells().begin(); it != vd.cells().end(); ++it) {
		const voronoi_diagram<double>::cell_type &cell = *it;
		const voronoi_diagram<double>::edge_type *edge = cell.incident_edge();

		// Ignore cells with infinite edges
		bool well_formed = true;
		do {
			if (edge->is_infinite()) {
				well_formed = false;
				break;
			}
			edge = edge->next();
		} while (edge != cell.incident_edge());

		if (!well_formed) {
			continue;
		}

		point_data<double> site = sites.at(cell.source_index());
		Tile *t = new Tile(site.x(), site.y());
		do {
			const voronoi_diagram<double>::edge_type::voronoi_vertex_type *v1 = edge->vertex0();
			const voronoi_diagram<double>::edge_type::voronoi_vertex_type *v2 = edge->vertex1();

			// Retrieve twin edge if it already exists
			Edge *e = edge_map.count(edge->twin()) > 0 ? edge_map.at(edge->twin()) : new Edge();

			Corner *c1, *c2;
			if (corner_map.count(v1) > 0) {
				c1 = corner_map.at(v1);
			} else {
				int x1 = std::max(0.0, std::min(m_size[0], v1->x()));
				int y1 = std::max(0.0, std::min(m_size[1], v1->y()));
				c1 =  new Corner(x1, y1);
			}

			if (corner_map.count(v2) > 0) {
				c2 = corner_map.at(v2);
			} else {
				int x2 = std::max(0.0, std::min(m_size[0], v2->x()));
				int y2 = std::max(0.0, std::min(m_size[1], v2->y()));
				c2 = new Corner(x2, y2);
			}

			// Check if we have seen the edges twin already
			if (edge_map.count(edge->twin()) == 0) {
				// First time seeing edge
				c1->protrudes().push_back(e);
				c2->protrudes().push_back(e);

				c1->adjacent().push_back(c2);
				c2->adjacent().push_back(c1);

				e->v_edge().first = c1;
				e->v_edge().second = c2;

				e->d_edge().first = t;

				m_edges.push_back(e);
				edge_map.insert(std::make_pair(edge, e));
			} else {
				// Second time seeing edge
				e->d_edge().first->neighbours().push_back(t);
				t->neighbours().push_back(e->d_edge().first);

				e->d_edge().second = t;
			}
			c2->touches().push_back(t);
			c2->touches().push_back(t);

			t->edges().push_back(e);
			t->corners().push_back(c1);
			t->corners().push_back(c2);

			if (corner_map.count(v1) == 0) {
				m_corners.push_back(c1);
				corner_map.insert(std::make_pair(v1, c1));
			}

			if (corner_map.count(v2) == 0) {
				m_corners.push_back(c2);
				corner_map.insert(std::make_pair(v2, c2));
			}

			edge = edge->next();
		} while (edge != cell.incident_edge());
		m_tiles.push_back(t);
	}

	UpdateState(STATE_ASSIGN_BORDERS);
}

void HexTileMap::GenerateTiles(void) 
{
	UpdateState(STATE_ASSIGN_BORDERS);
}

void GridTileMap::GenerateTiles(void) 
{
	int num_columns = static_cast<int>(std::floor(m_size[0] / m_cellSize));
	int num_rows = static_cast<int>(std::floor(m_size[1] / m_cellSize));

	// Create corners
	for (int row = 0; row <= num_rows; ++row) {
		for (int column = 0; column <= num_columns; ++column) {
			Corner *corner = new Corner(column * m_cellSize, row * m_cellSize);

			m_corners.push_back(corner);
		}
	}

	// Create Tiles
	for (int row = 0; row < num_rows; ++row) {
		for (int column = 0; column < num_columns; ++column) {
			Tile *tile = new Tile(column * m_cellSize + m_cellSize / 2, row * m_cellSize + m_cellSize / 2);

			tile->corners().push_back(m_corners.at(row * (num_columns + 1) + column));
			tile->corners().push_back(m_corners.at(row * (num_columns + 1) + column + 1));
			tile->corners().push_back(m_corners.at((row + 1) * (num_columns + 1) + column + 1));
			tile->corners().push_back(m_corners.at((row + 1) * (num_columns + 1) + column));
			
			m_corners.at(row * (num_columns + 1) + column)->touches().push_back(tile);
			m_corners.at((row + 1) * (num_columns + 1) + column)->touches().push_back(tile);
			m_corners.at((row + 1) * (num_columns + 1) + column + 1)->touches().push_back(tile);
			m_corners.at(row * (num_columns + 1) + column + 1)->touches().push_back(tile);

			m_tiles.push_back(tile);
		}
	}

	// Create edges
	for (int row = 0; row < num_rows; ++row) {
		for (int column = 0; column < num_columns; ++column) {
			if (column != num_columns - 1) {
				Edge *e1 = new Edge();
			
				e1->d_edge().first = m_tiles.at(row * num_columns + column);
				e1->d_edge().second = m_tiles.at(row * num_columns + column + 1);
				m_tiles.at(row * num_columns + column)->edges().push_back(e1);
				m_tiles.at(row * num_columns + column + 1)->edges().push_back(e1);

				m_tiles.at(row * num_columns + column)->neighbours().push_back(m_tiles.at(row * num_columns + column + 1));
				m_tiles.at(row * num_columns + column + 1)->neighbours().push_back(m_tiles.at(row * num_columns + column));

				e1->v_edge().first = m_corners.at(row * (num_columns + 1) + (column + 1));
				e1->v_edge().second = m_corners.at((row + 1) * (num_columns + 1) + (column + 1));
				m_corners.at(row * (num_columns + 1) + (column + 1))->protrudes().push_back(e1);
				m_corners.at((row + 1) * (num_columns + 1) + (column + 1))->protrudes().push_back(e1);

				m_corners.at(row * (num_columns + 1) + (column + 1))->adjacent().push_back(m_corners.at((row + 1) * (num_columns + 1) + (column + 1)));
				m_corners.at((row + 1) * (num_columns + 1) + (column + 1))->adjacent().push_back(m_corners.at(row * (num_columns + 1) + (column + 1)));

				m_edges.push_back(e1);
			}
			if (row != num_rows - 1) {
				Edge *e2 = new Edge();

				e2->d_edge().first = m_tiles.at(row * num_columns + column);
				e2->d_edge().second = m_tiles.at((row + 1) * num_columns + column);
				m_tiles.at(row * num_columns + column)->edges().push_back(e2);
				m_tiles.at((row + 1) * num_columns + column)->edges().push_back(e2);

				m_tiles.at(row * num_columns + column)->neighbours().push_back(m_tiles.at((row + 1) * num_columns + column));
				m_tiles.at((row + 1) * num_columns + column)->neighbours().push_back(m_tiles.at(row * num_columns + column));

				e2->v_edge().first = m_corners.at((row + 1) * (num_columns + 1) + column);
				e2->v_edge().second = m_corners.at((row + 1) * (num_columns + 1) + column + 1);
				m_corners.at((row + 1) * (num_columns + 1) + column)->protrudes().push_back(e2);
				m_corners.at((row + 1) * (num_columns + 1) + column + 1)->protrudes().push_back(e2);

				m_corners.at((row + 1) * (num_columns + 1) + column)->adjacent().push_back(m_corners.at((row + 1) * (num_columns + 1) + column + 1));
				m_corners.at((row + 1) * (num_columns + 1) + column + 1)->adjacent().push_back(m_corners.at((row + 1) * (num_columns + 1) + column));

				m_edges.push_back(e2);
			}
		}
	}

	// Join borders
	for (int row = 0; row < num_rows; ++row) {
		Corner *c1 = m_corners.at(row * (num_columns + 1));
		Corner *c2 = m_corners.at(row * (num_columns + 1) + num_columns);
		Tile *t1 = m_tiles.at(row * num_columns);
		Tile *t2 = m_tiles.at(row * num_columns + (num_columns - 1));
		Edge *e1 = new Edge();
		Edge *e2 = new Edge();

		e1->d_edge().first = t1;
		e1->v_edge().first = c1;
		e1->v_edge().second = m_corners.at((row + 1) * (num_columns + 1));

		e1->d_edge().first = t2;
		e2->v_edge().first = c2;
		e2->v_edge().second = m_corners.at((row + 1) * (num_columns + 1) + num_columns);

		c1->adjacent().push_back(m_corners.at((row + 1) * (num_columns + 1)));
		c2->adjacent().push_back(m_corners.at((row + 1) * (num_columns + 1) + num_columns));

		t1->edges().push_back(e1);
		t2->edges().push_back(e2);

		m_edges.push_back(e1);
		m_edges.push_back(e2);
	}
	for (int column = 0; column < num_columns; ++column) {
		Corner *c1 = m_corners.at(column);
		Corner *c2 = m_corners.at(num_rows * (num_columns + 1) + column);
		Tile *t1 = m_tiles.at(column);
		Tile *t2 = m_tiles.at((num_rows - 1) * num_columns + column);
		Edge *e1 = new Edge();
		Edge *e2 = new Edge();

		e1->d_edge().first = t1;
		e1->v_edge().first = c1;
		e1->v_edge().second = m_corners.at(column + 1);

		e2->d_edge().first = t1;
		e2->v_edge().first = c2;
		e2->v_edge().second = m_corners.at(num_rows * (num_columns + 1) + column + 1);

		c1->adjacent().push_back(m_corners.at(column + 1));
		c2->adjacent().push_back(m_corners.at(num_rows * (num_columns + 1) + column + 1));

		t1->edges().push_back(e1);
		t2->edges().push_back(e2);

		m_edges.push_back(e1);
		m_edges.push_back(e2);
	}

	UpdateState(STATE_ASSIGN_BORDERS);
}

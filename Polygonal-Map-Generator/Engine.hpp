#ifndef _ENGINE_HPP_
#define _ENGINE_HPP_

#include "Algebra.hpp"
#include "OpenGL.hpp"

class TileMap;

class Engine 
{
public:
	Engine(const Vector2D &size) 
		: m_size(size), m_done(false), m_tileMap(nullptr) {};

	void Init(void);
	void Run(void);

	void HandleInput(void);
	void HandleKeyEvent(int key, int scancode, int action, int mods);

private:
	void InitWindow(void);
	void InitOpenGL(void);
	void InitSystems(void);

	void Step(const double deltaTime);
	void Draw(void);

	Vector2D m_size;

	bool m_done;

	TileMap *m_tileMap;
};

#endif
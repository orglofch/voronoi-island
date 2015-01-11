#include "Engine.hpp"

#include <iostream>

#include "WindowManager.hpp"
#include "EntityFactory.hpp"
#include "SystemManager.hpp"
#include "EntityManager.hpp"
#include "TileMap.hpp"
#include "Entity.hpp"

#include "CollisionSystem.hpp"
#include "MovementSystem.hpp"
#include "RenderSystem.hpp"

using namespace std;

void Engine::Init(void) 
{
	InitWindow();
	InitOpenGL();
	InitSystems();

	m_tileMap = new VoronoiTileMap(m_size, 20000);
	m_tileMap->Generate();
}

void Engine::InitWindow(void) 
{
	eWindowManager.window().create(sf::VideoMode((int)m_size[0], (int)m_size[1]), "Story Board Heroes");
	eWindowManager.window().setVerticalSyncEnabled(true);
}

void Engine::InitOpenGL(void) 
{
	glViewport(0, 0, m_size[0], m_size[1]);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(0.0f, m_size[0], 0.0f, m_size[1], 1.0f, -1000.0f);
	gluPerspective(60.0, m_size[0]/m_size[1], 1.0f, 10000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(m_size[0]/2.0, m_size[1]/2.0, -800.0, 
		m_size[0] / 2.0, m_size[1] / 2.0, 0.0,
		0.0, 1.0, 0.0); 

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT); // TODO make this GL_BACK

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat lightPosition[] = { m_size[0] / 2.0, m_size[1] / 2.0, 500.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glShadeModel(GL_FLAT);

	glEnable (GL_BLEND);
	//glBlendFunc (GL_ONE, GL_ONE);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::InitSystems(void)
{
	eSystemManager.AddSystem(new RenderSystem());
	eSystemManager.AddSystem(new MovementSystem());
	eSystemManager.AddSystem(new CollisionSystem());
	
	for (int i = 0; i < 100; ++i) {
		EntityFactory::CreateEntity(ENTITY_PLAYER);
	}
	EntityFactory::CreateEntity(ENTITY_COLLIDER);
}

void Engine::Run(void) 
{
	sf::Clock clock;
	double timeDelta = 1000/60;
	while (eWindowManager.window().isOpen() && !m_done) {

		HandleInput();
		Step(clock.restart().asMilliseconds());
		Draw();
	}
}

void Engine::HandleInput(void)
{
	sf::Event event;
	while (eWindowManager.window().pollEvent(event)) {
		switch (event.type) 
		{
			case sf::Event::Closed:
				eWindowManager.window().close();
		}
	}
}

void Engine::HandleKeyEvent(int key, int scancode, int action, int mods) 
{
}

void Engine::Step(const double deltaTime) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_tileMap->Step(deltaTime);

	eSystemManager.Update(deltaTime);
}

void Engine::Draw(void)
{
	m_tileMap->RenderMap();

	eWindowManager.window().display();
}
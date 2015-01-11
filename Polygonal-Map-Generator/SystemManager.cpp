#include "SystemManager.hpp"

#include <iostream>

#include "System.hpp"

SystemManager eSystemManager;

SystemManager::~SystemManager()
{
	for (System *system : m_systems) {
		delete system;
	}
}

void SystemManager::AddSystem(System *system)
{
	system->Init();
	m_systems.push_back(system);
}

void SystemManager::Update(const double deltaTime)
{
	for (System *system : m_systems) {
		system->Update(deltaTime);
	}
}

#ifndef _SYSTEM_MANAGER_HPP_
#define _SYSTEM_MANAGER_HPP_

#include <vector>

class System;

class SystemManager
{
public:
	~SystemManager();

	void AddSystem(System *system);

	void Update(const double deltaTime);

private:
	std::vector<System*> m_systems;
};

extern SystemManager eSystemManager;

#endif
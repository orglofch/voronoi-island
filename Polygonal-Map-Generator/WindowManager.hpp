#ifndef _WINDOW_MANAGER_HPP_
#define _WINDOW_MANAGER_HPP_

#include "OpenGL.hpp"

class WindowManager
{
public:
	sf::RenderWindow &window() { return m_window; }
	const sf::RenderWindow &window() const { return m_window; }

private:
	sf::RenderWindow m_window;
};

extern WindowManager eWindowManager;

#endif
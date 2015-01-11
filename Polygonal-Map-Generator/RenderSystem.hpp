#ifndef _RENDER_SYSTEM_HPP_
#define _RENDER_SYSTEM_HPP_

#include "System.hpp"

class RenderSystem : public System
{
public:
	virtual void Update(const double deltaTime) override;
};

#endif
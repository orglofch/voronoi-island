#ifndef _LOGIC_COMPONENT_HPP_
#define _LOGIC_COMPONENT_HPP_

#include "Component.hpp"

#include "Message.hpp"

struct LogicComponent : public Component
{
	virtual void HandleMessage(const Message *message) {};
};

#endif
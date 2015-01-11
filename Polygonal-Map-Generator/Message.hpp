#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include "Entity.hpp"

class Message
{
public:
	enum Type {
		MESSAGE_COLLISION,
	};

	Type type() const { return m_type; }

protected:
	Message(Type type) : m_type(type) {}

	Type m_type;
};

class CollisionMessage : public Message {
public:
	CollisionMessage(Entity &_e1, Entity &_e2) : 
		Message(MESSAGE_COLLISION), e1(_e1), e2(_e2) {}

	Entity &e1;
	Entity &e2;
};

// TODO define macros for defining messages / use resource files

#endif
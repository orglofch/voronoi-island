#ifndef _MESSAGE_MANAGER_HPP_
#define _MESSAGE_MANAGER_HPP_

#include <unordered_map>
#include <queue>
#include <set>

#include "LogicComponent.hpp"
#include "System.hpp"
#include "Message.hpp"

class MessageManager
{
public:
	void Subscribe(System *system, Message::Type type);
	void Subscribe(LogicComponent *component, Message::Type type);

	void Unsubscribe(System *system, Message::Type type);
	void Unsubscribe(LogicComponent *component, Message::Type type);

	void SendMessage(const Message *message);

	void BroadcastMessage(const Message *message);

private:
	typedef std::unordered_map<Message::Type, std::set<System*> > SystemSubscriptionMap;
	typedef std::unordered_map<Message::Type, std::set<LogicComponent*> > ComponentSubscriptionMap;

	SystemSubscriptionMap m_system_subscriptions;
	ComponentSubscriptionMap m_component_subscriptions;
};

extern MessageManager eMessageManager;

#endif
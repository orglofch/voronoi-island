#include "MessageManager.hpp"

MessageManager eMessageManager;

void MessageManager::Subscribe(System *system, Message::Type type)
{
	m_system_subscriptions[type].insert(system);
}

void MessageManager::Subscribe(LogicComponent *component, Message::Type type)
{
	m_component_subscriptions[type].insert(component);
}

void MessageManager::Unsubscribe(System *system, Message::Type type)
{
	m_system_subscriptions[type].erase(system);
}

void MessageManager::Unsubscribe(LogicComponent *component, Message::Type type)
{
	m_component_subscriptions[type].erase(component);
}

void MessageManager::SendMessage(const Message *message)
{
	// TODO
}

void MessageManager::BroadcastMessage(const Message *message)
{
	for (System *system : m_system_subscriptions[message->type()]) {
		system->HandleMessage(message);
	}
	for (LogicComponent *component : m_component_subscriptions[message->type()]) {
		component->HandleMessage(message);
	}
	delete message;
}
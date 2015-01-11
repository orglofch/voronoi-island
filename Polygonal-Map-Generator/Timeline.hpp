#ifndef _TIMELINE_HPP_
#define _TIMELINE_HPP_

#include <list>

class Action;

class Timeline
{
public:
	void AddAction(Action *action);

	Action *NextAction();

private:
	std::list<Action*> m_actions;

	double m_total_duration;
};

#endif
#ifndef _TIMELINE_COMPONENT_HPP_
#define _TIMELINE_COMPONENT_HPP_

#include "Timeline.hpp"

#include "Component.hpp"

struct TimelineComponent : public Component
{
	Timeline timeline;
};

#endif
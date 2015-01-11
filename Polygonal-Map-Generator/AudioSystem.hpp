#ifndef _AUDIO_SYSTEM_HPP_
#define _AUDIO_SYSTEM_HPP_

#include "System.hpp"

class AudioSystem : public System
{
public:
	virtual void Update(const double deltaTime) override;
};

#endif
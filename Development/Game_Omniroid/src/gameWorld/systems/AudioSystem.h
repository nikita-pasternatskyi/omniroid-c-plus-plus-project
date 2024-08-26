#pragma once
#include "../src/audio/Audio.h"
#include "ISystem.h"

class AudioSystem : public ISystem
{
public:
	AudioSystem(Audio& audio);
	void Update(entt::registry& registry) override;
private:
	std::unique_ptr<Audio> _audio;
};
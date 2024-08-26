#pragma once
#include "glm/glm.hpp"
#include "Audio.h"
class SoundEffect
{
public:
	SoundEffect(Audio& audio, const char* eventPath, float reverbAffection);
	~SoundEffect();
	void Play();
	void Stop();
	void ChangePosition(glm::vec3 newPosition);
private:
	FMOD_3D_ATTRIBUTES _attributes;
	FMOD::Studio::EventDescription* _eventDescription;
	FMOD::Studio::EventInstance* _eventInstance;
	glm::vec3 _position;
};
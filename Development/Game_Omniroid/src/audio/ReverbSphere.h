#pragma once
#include <glm/vec3.hpp>
#include "Audio.h"
class ReverbSphere
{
public:
	ReverbSphere(Audio& audio, FMOD_REVERB_PROPERTIES properties);
	~ReverbSphere();
    ReverbSphere* ChangePosition(glm::vec3 newPosition);
    ReverbSphere* ChangeMinDist(float value);
    ReverbSphere* ChangeMaxDist(float value);

    ReverbSphere* ChangeDecayTime(float value);
    ReverbSphere* ChangeEarlyDelay(float value);
    ReverbSphere* ChangeLateDelay(float value);
    ReverbSphere* ChangeHFReference(float value);
    ReverbSphere* ChangeHFDecayRatio(float value);
    ReverbSphere* ChangeDiffusion(float value);
    ReverbSphere* ChangeDensity(float value);
    ReverbSphere* ChangeLowShelfFrequency(float value);
    ReverbSphere* ChangeLowShelfGain(float value);
    ReverbSphere* ChangeHighCut(float value);
    ReverbSphere* ChangeEarlyLateMix(float value);
    ReverbSphere* ChangeWetLevel(float value);

private:
	FMOD_REVERB_PROPERTIES Properties;
	FMOD::Reverb3D* _reverb;
	glm::vec3 _position;
    float _minDist;
    float _maxDist;
};
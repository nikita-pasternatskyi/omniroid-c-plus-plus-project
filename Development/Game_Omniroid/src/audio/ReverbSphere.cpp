#include "ReverbSphere.h"
#include "Audio.h"

ReverbSphere::ReverbSphere(Audio& audio, FMOD_REVERB_PROPERTIES properties)
{
    audio.CreateReverb3D(&_reverb);
    Properties = properties;
    FMODCall(_reverb->setProperties(&Properties));
    ChangePosition(glm::vec3(0.0f, 0.0f, 2.0f))->ChangeMinDist(10.0f)->ChangeMaxDist(20.0f);
}

ReverbSphere::~ReverbSphere()
{
    FMODCall(_reverb->release());
}

ReverbSphere* ReverbSphere::ChangePosition(glm::vec3 newPosition)
{
    _position = newPosition;
    FMOD_VECTOR pos = { _position.x, _position.y, _position.y };
    FMODCall(_reverb->set3DAttributes(&pos, _minDist, _maxDist));
    return this;
}

ReverbSphere* ReverbSphere::ChangeMinDist(float value)
{
    _minDist = value;
    FMOD_VECTOR pos = { _position.x, _position.y, _position.y };
    FMODCall(_reverb->set3DAttributes(&pos, _minDist, _maxDist));
    return this;
}

ReverbSphere* ReverbSphere::ChangeMaxDist(float value)
{
    _maxDist = value;
    FMOD_VECTOR pos = { _position.x, _position.y, _position.y };
    FMODCall(_reverb->set3DAttributes(&pos, _minDist, _maxDist));
    return this;
}

ReverbSphere* ReverbSphere::ChangeDecayTime(float value)
{
    Properties.DecayTime = value;
    FMODCall(_reverb->setProperties(&Properties));
    return this;
}

ReverbSphere* ReverbSphere::ChangeEarlyDelay(float value)
{
    Properties.EarlyDelay = value;
    FMODCall(_reverb->setProperties(&Properties));
    return this;
}

ReverbSphere* ReverbSphere::ChangeLateDelay(float value)
{
    Properties.LateDelay = value;
    FMODCall(_reverb->setProperties(&Properties));
    return this;
}

ReverbSphere* ReverbSphere::ChangeHFReference(float value)
{
    Properties.HFReference = value;
    FMODCall(_reverb->setProperties(&Properties));
    return this;
}

ReverbSphere* ReverbSphere::ChangeHFDecayRatio(float value)
{
    Properties.HFDecayRatio = value;
    FMODCall(_reverb->setProperties(&Properties));
    return this;
}

ReverbSphere* ReverbSphere::ChangeDiffusion(float value)
{
    Properties.Diffusion = value;
    FMODCall(_reverb->setProperties(&Properties));
    return this;
}

ReverbSphere* ReverbSphere::ChangeDensity(float value)
{
    Properties.Density = value;
    FMODCall(_reverb->setProperties(&Properties));
    return this;
}

ReverbSphere* ReverbSphere::ChangeLowShelfFrequency(float value)
{
    Properties.LowShelfFrequency = value;
    FMODCall(_reverb->setProperties(&Properties));
    return this;
}

ReverbSphere* ReverbSphere::ChangeLowShelfGain(float value)
{
    Properties.LowShelfGain = value;
    FMODCall(_reverb->setProperties(&Properties));
    return this;
}

ReverbSphere* ReverbSphere::ChangeHighCut(float value)
{
    Properties.HighCut = value;
    FMODCall(_reverb->setProperties(&Properties));
    return this;
}

ReverbSphere* ReverbSphere::ChangeEarlyLateMix(float value)
{
    Properties.EarlyLateMix = value;
    FMODCall(_reverb->setProperties(&Properties));
    return this;
}

ReverbSphere* ReverbSphere::ChangeWetLevel(float value)
{
    Properties.WetLevel = value;
    FMODCall(_reverb->setProperties(&Properties));
    return this;
}

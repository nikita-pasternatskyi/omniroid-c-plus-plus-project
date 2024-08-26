#include "SoundEffect.h"
#include "Audio.h"

SoundEffect::SoundEffect(Audio& audio, const char* eventPath, float reverbAffection)
{
    audio.GetEvent(&_eventDescription, eventPath);
    FMODCall(_eventDescription->createInstance(&_eventInstance));
    _attributes = { { 0 } };
    _attributes.forward.z = 1.0f;
    _attributes.up.y = 1.0f;
    ChangePosition(glm::vec3(0, 0, 2));
    _eventInstance->setReverbLevel(0, reverbAffection);
}

SoundEffect::~SoundEffect()
{
    FMODCall(_eventInstance->release());
}

void SoundEffect::Play()
{
    FMODCall(_eventInstance->start());
}

void SoundEffect::Stop()
{
    FMODCall(_eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT));
}

void SoundEffect::ChangePosition(glm::vec3 newPosition)
{
    _attributes.position.x = -newPosition.x;
    _attributes.position.y = newPosition.y;
    _attributes.position.z = 0;
    FMODCall(_eventInstance->set3DAttributes(&_attributes));
}

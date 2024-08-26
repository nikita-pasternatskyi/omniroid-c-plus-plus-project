#include "Audio.h"
#include "../debug/DebugLogger.h"
#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"

bool FMODCheckError(FMOD_RESULT result, const char* file, int line)
{
    if (result != FMOD_OK)
    {
        LOG_ERROR("[FMOD]::" << FMOD_ErrorString(result));
        return false;
    }
    return true;
}

Audio::Audio(const char* masterBank, const char* masterStringsBank)
{
    FMODCall(FMOD::Studio::System::create(&_system));
    FMODCall(_system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));
    FMODCall(_system->getCoreSystem(&_coreSystem));
    LoadBank(&_masterBank, masterBank);
    LoadBank(&_stringsBank, masterStringsBank);
}

void Audio::Update()
{
    FMODCall(_system->update());
}

void Audio::Release()
{
    FMODCall(_system->release());
}

void Audio::LoadBank(FMOD::Studio::Bank** bank, const char* path)
{
    FMODCall(_system->loadBankFile(path, FMOD_STUDIO_LOAD_BANK_NORMAL, bank));
}

void Audio::GetEvent(FMOD::Studio::EventDescription** eventDescription, const char* path)
{
    FMODCall(_system->getEvent(path, eventDescription));
    LOG(eventDescription);
}

void Audio::GetListenerAttributes(int listener, FMOD_3D_ATTRIBUTES* attributes, FMOD_VECTOR* attenuationposition)
{
    FMODCall(_system->getListenerAttributes(listener, attributes, attenuationposition));
}

void Audio::SetListenerAttributes(int listener, const FMOD_3D_ATTRIBUTES* attributes, const FMOD_VECTOR* attenuationposition)
{
    FMODCall(_system->setListenerAttributes(listener, attributes, attenuationposition));
}

void Audio::CreateReverb3D(FMOD::Reverb3D** reverb3D)
{
    FMODCall(_coreSystem->createReverb3D(reverb3D));
}

void Audio::SetReverbProperties(FMOD_REVERB_PROPERTIES* properties)
{
    _coreSystem->setReverbProperties(0, properties);
}

#pragma once
#include "fmod/fmod_studio.hpp"
#include "../debug/DebugLogger.h"
#include "glm/glm.hpp"

class GameWorld;
class AudioListener;

bool FMODCheckError(FMOD_RESULT result, const char* file, int line);
#define FMODCall(_result) ASSERT(FMODCheckError(_result, __FILE__, __LINE__))

class Audio
{
public:
	Audio() = default;
	Audio(const char* masterBank, const char* masterStringsBank);
	void Update();
	void Release();
	void LoadBank(FMOD::Studio::Bank** bank, const char* path);
	void GetEvent(FMOD::Studio::EventDescription** eventDescription, const char* path);
	void GetListenerAttributes(int listener, FMOD_3D_ATTRIBUTES* attributes, FMOD_VECTOR* attenuationposition = 0);
	void SetListenerAttributes(int listener, const FMOD_3D_ATTRIBUTES* attributes, const FMOD_VECTOR* attenuationposition = 0);
	void CreateReverb3D(FMOD::Reverb3D** reverb3D);
	void SetReverbProperties(FMOD_REVERB_PROPERTIES* properties);


private:
	FMOD::Studio::Bank* _masterBank;
	FMOD::Studio::Bank* _stringsBank;
	FMOD::System* _coreSystem;
	FMOD::Studio::System* _system;
};
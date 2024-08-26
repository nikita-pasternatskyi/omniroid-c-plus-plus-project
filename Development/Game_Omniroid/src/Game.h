#pragma once
#include "fmod/fmod.hpp"
#include "fmod/fmod_studio.hpp"
#include "game/Demo.h"

#include "rendering/Renderer.h"
#include "input/Input.h"
#include "audio/Audio.h"

#include "gameWorld/systems/RenderingSystem.h"
#include "gameWorld/systems/AudioSystem.h"

class Game
{
public:
	Game();
	~Game();
	bool ShouldRun();
	void Update();
	int ScreenSize = 4;
	const int RENDER_RESOLUTION_X = 480;
	const int RENDER_RESOLUTION_Y = 270;

	inline int GetScreenResolutionX() const { return RENDER_RESOLUTION_X * ScreenSize; }
	inline int GetScreenResolutionY() const { return RENDER_RESOLUTION_Y * ScreenSize; }

private:
	Renderer _renderer;
	Input _input;
	Audio _audio;
	Demo _demo;


};


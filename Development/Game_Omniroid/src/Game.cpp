#include "Game.h"

#include "time/Time.h"
#include "gameWorld/systems/RenderingSystem.h"
#include "gameWorld/systems/AudioSystem.h"

Game::Game()
    :   _renderer(RENDER_RESOLUTION_X, RENDER_RESOLUTION_Y, ScreenSize),
        _input(_renderer.WindowPtr,"data/default/input.inputMap", "data/user/input.inputMap"),
        _audio("assets/audio/Master.bank", "assets/audio/Master.strings.bank"),
        _demo(*this, _renderer, _audio, _input)
{
}

Game::~Game()
{
}

bool Game::ShouldRun()
{
    return _renderer.IsWindowAlive();
}

void Game::Update()
{
    _audio.Update();
    Time::Update();
    _demo.Update();

    _renderer.SwapBuffers();

    _input.Update();
}


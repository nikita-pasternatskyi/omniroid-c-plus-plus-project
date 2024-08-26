#pragma once
#include "AnimationFrame.h"
#include <vector>

struct Animation
{
	std::vector<AnimationFrame> Frames;
	bool Looped;
	int LoopFrame;
};


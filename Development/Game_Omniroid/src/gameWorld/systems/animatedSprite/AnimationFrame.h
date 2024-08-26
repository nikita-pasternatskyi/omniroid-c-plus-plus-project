#pragma once

#include "glm/glm.hpp"
struct AnimationFrame
{
	glm::vec2 Position;
	glm::vec2 Offset;
	glm::vec2 Size;
	int Duration;
};
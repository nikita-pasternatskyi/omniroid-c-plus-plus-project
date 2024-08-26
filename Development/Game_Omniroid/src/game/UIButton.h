#pragma once

#include "glm/glm.hpp"
#include <vector>


struct UIButton
{
	UIButton(const glm::vec2& size, const std::function<void()>& callBack)
		: Size(size), Selected(false), Callback(callBack)
	{

	}
	std::function<void()> Callback;
	glm::vec2 Size;
	bool Selected;
};
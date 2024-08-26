#pragma once
#include "glm/glm.hpp"

struct TransformComponent
{
	TransformComponent() = default;
	TransformComponent(const glm::vec3& position, const glm::vec2& scale, float rotation)
	{
		Position = position;
		Scale = scale;
		Rotation = rotation;
	}

	glm::vec3 Position;
	glm::vec2 Scale;
	float Rotation;

	inline glm::vec2 TopLeft()
	{
		return glm::vec2(Position.x - Scale.x, Position.y + Scale.y);
	}

	inline glm::vec2 TopRight()
	{
		return glm::vec2(Position.x + Scale.x, Position.y + Scale.y);
	}

	inline glm::vec2 BottomLeft()
	{
		return glm::vec2(Position.x - Scale.x, Position.y - Scale.y);
	}

	inline glm::vec2 BottomRight()
	{
		return glm::vec2(Position.x + Scale.x, Position.y - Scale.y);
	}
};
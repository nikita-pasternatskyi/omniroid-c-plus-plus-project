#pragma once
#include "glm/glm.hpp"

struct LightComponent
{
	LightComponent() = default;
	LightComponent(const glm::vec3& color, float intensity, bool castRays, float sineInfluence)
		: Color(color), Intensity(intensity), CastRays(castRays), SineInfluence(sineInfluence)
	{}

	glm::vec3 Color;
	float Intensity;
	bool CastRays;
	float SineInfluence;
};
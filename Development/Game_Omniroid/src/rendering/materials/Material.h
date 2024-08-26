#pragma once
#include <vector>
#include <string>
#include "../ShaderParameter.h"
struct Material
{
	Material(const glm::vec4 tint)
		: Tint(tint)
	{

	}
	enum MaterialType
	{
		WORLD, SCREEN
	};
	glm::vec4 Tint;
	glm::vec2 UVOffset = glm::vec2(0,0);
	glm::vec2 UVScale = glm::vec2(1,1);
	virtual inline MaterialType GetMaterialType() = 0;
	virtual inline std::string GetShaderPath() = 0;
	virtual inline std::vector<std::string> GetTexturePaths() = 0;
	virtual inline std::vector<ShaderParameter> GetShaderParameters() = 0;
};
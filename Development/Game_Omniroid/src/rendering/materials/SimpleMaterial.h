#pragma once
#include "Material.h"
#include <glm/vec4.hpp>
#include <string>
struct SimpleMaterial : Material
{
	SimpleMaterial(const glm::vec4& tint, const char* texturePath)
		: Material(tint), AlbedoTexturePath(texturePath)
	{
	}

	inline MaterialType GetMaterialType() override { return MaterialType::WORLD; }
	inline std::string GetShaderPath() override { return "assets/shaders/simple.shader"; };
	inline std::vector<ShaderParameter> GetShaderParameters() override
	{
		return
		{ 
			ShaderParameter{ShaderParameter::TEXTURE,	"AlbedoTexture",		&AlbedoTexturePath},
			ShaderParameter{ShaderParameter::VEC4,	"Tint",	&Tint}
		};
	}

	std::vector<std::string> GetTexturePaths() override
	{
		return
		{
			AlbedoTexturePath
		};
	}

	std::string AlbedoTexturePath = nullptr;
};
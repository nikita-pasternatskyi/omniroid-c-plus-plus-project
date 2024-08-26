#pragma once
#include "Material.h"
#include <glm/vec4.hpp>
#include <string>
struct MainMenuUIScrollMaterial : Material
{
	MainMenuUIScrollMaterial(const glm::vec4& tint, const char* texturePath, float scrollSpeed)
		: Material(tint), AlbedoTexturePath(texturePath), ScrollSpeed(scrollSpeed)
	{
	}

	inline MaterialType GetMaterialType() override { return MaterialType::WORLD; }
	inline std::string GetShaderPath() override { return "assets/shaders/main_menu_background_scroll.shader"; };
	inline std::vector<ShaderParameter> GetShaderParameters() override
	{
		return
		{ 
			ShaderParameter{ShaderParameter::TEXTURE,	"AlbedoTexture",		&AlbedoTexturePath},
			ShaderParameter{ShaderParameter::FLOAT,		"ScrollSpeed",			&ScrollSpeed},
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
	float ScrollSpeed;
};
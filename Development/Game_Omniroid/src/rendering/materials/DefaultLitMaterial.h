#pragma once
#include "Material.h"
#include <glm/vec4.hpp>
#include <string>
struct DefaultLitMaterial : Material
{
	DefaultLitMaterial(const char* albedo, const char* normal, const char* parameters,
		glm::vec4& tint, float emissionMultiplier)
		: Material(tint),AlbedoTexturePath(albedo), NormalMapTexturePath(normal), ParametersMap(parameters),
		EmissionMultiplier(emissionMultiplier)
	{

	}

	DefaultLitMaterial(const char* albedo, const char* normal, const char* parameters,
		glm::vec4 tint, float emissionMultiplier)
		: Material(tint), AlbedoTexturePath(albedo), NormalMapTexturePath(normal), ParametersMap(parameters),
		EmissionMultiplier(emissionMultiplier)
	{

	}

	inline MaterialType GetMaterialType() override { return MaterialType::WORLD; }
	inline std::string GetShaderPath() override { return "assets/shaders/default_lit.shader"; };
	inline std::vector<std::string> GetTexturePaths() override
	{
		return
		{
			AlbedoTexturePath,NormalMapTexturePath,ParametersMap
		};
	}
	inline std::vector<ShaderParameter> GetShaderParameters() override
	{
		return
		{
			ShaderParameter{ShaderParameter::TEXTURE,	"AlbedoTexture",		&AlbedoTexturePath},
			ShaderParameter{ShaderParameter::TEXTURE,	"NormalTexture",		&NormalMapTexturePath},
			ShaderParameter{ShaderParameter::TEXTURE,	"ParamsTexture",		&ParametersMap},
			ShaderParameter{ShaderParameter::FLOAT,		"EmissionMultiplier",	&EmissionMultiplier},
			ShaderParameter{ShaderParameter::FLOAT,		"LightAbsorption",		&LightAbsorption},
			ShaderParameter{ShaderParameter::VEC4,		"Tint",					&Tint }
		};
	}
	

	std::string AlbedoTexturePath = nullptr;
	std::string NormalMapTexturePath = nullptr;
	std::string ParametersMap = nullptr;
	float EmissionMultiplier;
	float LightAbsorption;
};
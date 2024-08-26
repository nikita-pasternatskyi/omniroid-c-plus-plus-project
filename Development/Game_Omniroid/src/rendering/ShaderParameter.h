#pragma once
#include "glm/glm.hpp"
struct ShaderParameter
{
	enum ShaderParameterType
	{
		TEXTURE,
		FLOAT,
		INT,
		VEC2,
		VEC3,
		VEC4,
		MAT4,
	};
	ShaderParameterType Type;
	std::string Name;
	void* Value;
	bool Static = true;

	inline float GetFloatValue()	{ return *(float*)Value; }
	inline float GetIntValue()		{ return *(int*)Value; }
	inline glm::vec2 GetVec2()		{ return *(glm::vec2*)Value; }
	inline glm::vec3 GetVec3()		{ return *(glm::vec3*)Value; }
	inline glm::vec4 GetVec4()		{ return *(glm::vec4*)Value; }
	inline glm::mat4 GetMat4()		{ return *(glm::mat4*)Value; }
	inline std::string GetString()  { return *(std::string*)Value; }
};
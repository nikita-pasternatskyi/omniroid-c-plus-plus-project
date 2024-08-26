#pragma once
#include "Texture.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

class Shader
{
public:
	Shader(const char* shaderPath);
	~Shader();

	void Use() const;
	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform2f(const std::string& name, float v0, float v1);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetMatrix4f(const std::string& name, glm::mat4 value);
	inline int GetShaderID() { return _id; }
	unsigned int CompileShader(int shaderType, const char* sourceCode) const;
	bool Destroyed = false;
private:
	unsigned int _id;
	int GetUniformLocation(const std::string& name);
	std::unordered_map<std::string, int> _uniformLocationCache;
};


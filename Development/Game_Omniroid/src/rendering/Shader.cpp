#include "Shader.h"
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

enum class ShaderType
{
	NONE = -1, VERTEX = 0, FRAGMENT = 1,
};

Shader::Shader(const char* shaderPath) 
	: _id(0)
{
	std::string vertexSrc;
	std::string fragmentSrc;
	std::ifstream shaderFile(shaderPath);

	ShaderType shaderType = ShaderType::NONE;

	std::string line;
	std::stringstream stringStream[2];
	while (getline(shaderFile, line))
	{
		if (line.find("#vertex") != std::string::npos)
		{
			shaderType = ShaderType::VERTEX;
		}
		else if (line.find("#fragment") != std::string::npos)
		{
			shaderType = ShaderType::FRAGMENT;
		}
		else if (shaderType != ShaderType::NONE)
		{
			stringStream[(int)shaderType] << line << std::endl;
		}
	}
	vertexSrc = stringStream[(int)ShaderType::VERTEX].str();
	fragmentSrc = stringStream[(int)ShaderType::FRAGMENT].str();
	const char* vertexShaderCode = vertexSrc.c_str();
	const char* fragmentShaderCode = fragmentSrc.c_str();

	unsigned int vertexShader = CompileShader((int)ShaderType::VERTEX, vertexShaderCode);
	unsigned int fragmentShader = CompileShader((int)ShaderType::FRAGMENT, fragmentShaderCode);

	GLCall(_id = glCreateProgram());
	GLCall(glAttachShader(_id, vertexShader));
	GLCall(glAttachShader(_id, fragmentShader));
	GLCall(glLinkProgram(_id));

	int success;
	char infoLog[512];
	GLCall(glGetProgramiv(_id, GL_LINK_STATUS, &success));
	if (!success)
	{
		GLCall(glGetProgramInfoLog(_id, 512, NULL, infoLog));
		LOG_ERROR("[OPENGL]::[SHADER]::[LINKING_ERROR]::" << infoLog);
	}
	
	GLCall(glDeleteShader(vertexShader));
	GLCall(glDeleteShader(fragmentShader));
}

Shader::~Shader()
{
	Destroyed = true;
	//GLCall(glDeleteProgram(_id));
}

unsigned int Shader::CompileShader(int shaderType, const char* sourceCode) const
{
	int success;
	char infoLog[512];
	unsigned int shader = 0;
	switch (shaderType)
	{
		case (int)ShaderType::FRAGMENT: GLCall(shader = glCreateShader(GL_FRAGMENT_SHADER)); break;
		case (int)ShaderType::VERTEX: GLCall(shader = glCreateShader(GL_VERTEX_SHADER)); break;
	}
	GLCall(glShaderSource(shader, 1, &sourceCode, NULL));
	GLCall(glCompileShader(shader));
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLCall(glGetShaderInfoLog(shader, 512, NULL, infoLog));
		LOG_ERROR("[OPENGL]::[SHADER]::[COMPILING_ERROR]::" << infoLog);
	}
	return shader;
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (_uniformLocationCache.find(name) != _uniformLocationCache.end())
		return _uniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(_id, name.c_str()));
	if (location == -1) 
	{
		LOG_WARNING("[OPENGL]::[SHADER]::[UNIFORM]::" << name << " does not exist!");
	}
	
	_uniformLocationCache[name]= location;
	return location;
}

void Shader::Use() const
{
	GLCall(glUseProgram(_id));
}

void Shader::SetBool(const std::string& name, bool value)
{
	GLCall(glUniform1i(GetUniformLocation(name), (int)value));
}

void Shader::SetInt(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetFloat(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
	GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetMatrix4f(const std::string& name, glm::mat4 value)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value)));
}
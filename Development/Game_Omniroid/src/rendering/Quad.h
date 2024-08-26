#pragma once
#include <array>
#include "Vertex.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
struct Quad
{
	Quad() {}

	Quad(glm::vec3& scale, glm::vec3& position, float rotation)
	{
		for (int i = 0; i < 4; i++)
		{
			auto vertex = Vertices[i];
			glm::vec3 pos = glm::vec3(vertex.Position.x, vertex.Position.y, 0);
			glm::mat4 transformation = glm::mat4(1.0f);
			transformation = glm::translate(transformation, glm::vec3(vertex.Position.x, vertex.Position.y, 0));
			transformation = glm::scale(transformation, scale);
			transformation = glm::rotate(transformation, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
			transformation = glm::translate(transformation, position);
			vertex.Position.x = transformation[3].x;
			vertex.Position.y = transformation[3].y;
		}
	};
	
	Quad(glm::vec2& scale, glm::vec2& position, float rotation)
	{
		for (int i = 0; i < 4; i++)
		{
			auto vertex = Vertices[i];
			glm::vec3 pos = glm::vec3(vertex.Position.x, vertex.Position.y, 0);
			glm::mat4 transformation = glm::mat4(1.0);
			transformation = glm::translate(transformation, glm::vec3(vertex.Position.x, vertex.Position.y, 0));
			transformation = glm::scale(transformation, glm::vec3(scale.x, scale.y, 1));
			transformation = glm::rotate(transformation, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
			transformation = glm::translate(transformation, glm::vec3(position.x, position.y, 0));
			vertex.Position.x = transformation[3].x;
			vertex.Position.y = transformation[3].y;
		}
	};

	Vertex Vertices[4]
	{
			Vertex{ { -1.0f, -1.0f }, { 0.0f,0.0f }, 0 },
			Vertex{ {  1.0f, -1.0f }, { 1.0f,0.0f }, 0 },
			Vertex{ {  1.0f,  1.0f }, { 1.0f,1.0f }, 0 },
			Vertex{ { -1.0f,  1.0f }, { 0.0f,1.0f }, 0 }
	};
	unsigned int Indices[6]{ 0,1,3,1,2,3 };
};
#pragma once
#include "../src/rendering/VertexArray.h"
#include "../src/rendering/VertexBuffer.h"
#include "../src/rendering/IndexBuffer.h"
#include "../src/rendering/Shader.h"
#include "../src/rendering/Quad.h"
#include "../src/rendering/materials/Material.h"
#include <memory>
#include <vector>
#include "../../rendering/RenderingQuad.h"

struct SpriteRendererComponent
{
	SpriteRendererComponent(Material* material);
	~SpriteRendererComponent();
	RenderingQuad Quad;
	Material* SpriteMaterial;
	Shader SpriteShader;
	std::vector<Texture*> SpriteTextures;
	glm::vec2 Size = { 0,0 };
};
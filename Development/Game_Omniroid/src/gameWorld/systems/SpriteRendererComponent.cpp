#include "SpriteRendererComponent.h"

SpriteRendererComponent::SpriteRendererComponent(Material* material)
	: Quad(), SpriteMaterial(material), SpriteShader(SpriteMaterial->GetShaderPath().c_str())
{
	auto texturePaths = SpriteMaterial->GetTexturePaths();

	for (int i = 0; i < texturePaths.size(); i++)
	{
		auto texture = new Texture(texturePaths[i].c_str());
		SpriteTextures.push_back(texture);
		glm::vec2 texSize = { texture->GetWidth(), texture->GetHeight() };
		auto l = texSize.x * texSize.x + texSize.y * texSize.y;
		auto l2 = Size.x * Size.x + Size.y * Size.y;

		if (l > l2)
		{
			Size = texSize;
		}
	}
}

SpriteRendererComponent::~SpriteRendererComponent()
{
}

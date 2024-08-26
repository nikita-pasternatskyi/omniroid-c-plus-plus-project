#pragma once
#include "../src/rendering/Renderer.h"
#include "TransformComponent.h"
#include <memory>
#include "ISystem.h"

class RenderingSystem : public ISystem
{
public:
	RenderingSystem(Renderer& renderer);
	void Update(entt::registry& registry) override;
private:
	glm::mat4 GetTransformationMatrix(TransformComponent& transformComponent);
	glm::mat4 GetTransformationMatrix(glm::vec3 position, glm::vec2 scale, float rotation);
	Renderer& _renderer;
};
#include "RenderingSystem.h"
#include "SpriteRendererComponent.h"
#include "ForwardRendererComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"
#include "animatedSprite/AnimatedSpriteRendererComponent.h"
#include "../../time/Time.h"

RenderingSystem::RenderingSystem(Renderer& renderer)
	: _renderer(renderer)
{
}

void RenderingSystem::Update(entt::registry& registry)
{
	auto cameraView = registry.view<CameraComponent, TransformComponent>();
	for (auto entity : cameraView)
	{
		auto& transform = cameraView.get<TransformComponent>(entity);
		auto& camera = cameraView.get<CameraComponent>(entity);
		_renderer.ViewMatrix = GetTransformationMatrix(transform);
		_renderer.AspectRatio = camera.AspectRatio;
		_renderer.OrthoScale = camera.OrthoScale;
	}

	auto lightView = registry.view<LightComponent, TransformComponent>();
	int lightIdx = 0;
	for (auto entity : lightView)
	{
		auto [lightComponent, transform] = registry.get<LightComponent, TransformComponent>(entity);
		_renderer.UpdateLight(++lightIdx, transform.Position, lightComponent.Color, lightComponent.Intensity, lightComponent.SineInfluence, lightComponent.CastRays);
	}
	
	auto animatedSpriteRendererView = registry.view<AnimatedSpriteRendererComponent, SpriteRendererComponent, TransformComponent>();
	for (auto entity : animatedSpriteRendererView)
	{
		auto& renderer = animatedSpriteRendererView.get<AnimatedSpriteRendererComponent>(entity);
		auto& spriteRenderer = animatedSpriteRendererView.get<SpriteRendererComponent>(entity);
		auto& transform = animatedSpriteRendererView.get<TransformComponent>(entity);
		auto* material = spriteRenderer.SpriteMaterial;

		auto currentAnimationName = renderer.GetCurrentAnimation();
		if (currentAnimationName.empty())
		{
			continue;
		}

		auto& currentAnimation = renderer.Animations[renderer.GetCurrentAnimation()];
		auto& currentFrame = currentAnimation.Frames[renderer.CurrentAnimationFrame];
		int lastFrameIndex = currentAnimation.Frames.size()-1;

		renderer.CurrentTimer += Time::DeltaTime * 1000;

		if (renderer.CurrentTimer >= currentFrame.Duration)
		{
			renderer.CurrentAnimationFrame++;
			renderer.CurrentTimer = 0;
			if (renderer.CurrentAnimationFrame > lastFrameIndex) //if this is last frame
			{
				if (currentAnimation.Looped)
				{
					renderer.CurrentAnimationFrame = currentAnimation.LoopFrame;
				}
				else
				{
					renderer.CurrentAnimationFrame = lastFrameIndex;
				}
			}
		}

		material->UVScale = currentFrame.Size / spriteRenderer.Size;
		float offsetX = currentFrame.Position.x / spriteRenderer.Size.x;
		float offsetY = 1 - ((currentFrame.Position.y + currentFrame.Size.y) / spriteRenderer.Size.y);
		material->UVOffset.x = offsetX;
		material->UVOffset.y = offsetY;
	}

	_renderer.BeginDeferredRendering();
	auto deferredRenderingView = registry.view<SpriteRendererComponent, TransformComponent>(entt::exclude<ForwardRendererComponent>);
	for (auto entity : deferredRenderingView)
	{
		auto [transform, spriteRenderer] = deferredRenderingView.get<TransformComponent, SpriteRendererComponent>(entity);
		spriteRenderer.SpriteShader.Use();
		if (spriteRenderer.SpriteMaterial == nullptr)
		{
			continue;
		}
		auto shaderParams = spriteRenderer.SpriteMaterial->GetShaderParameters();
		for (int i = 0; i < shaderParams.size(); i++)
		{
			auto parameter = shaderParams[i];
			switch (parameter.Type)
			{
			case ShaderParameter::TEXTURE:
				spriteRenderer.SpriteShader.SetUniform1i(parameter.Name, i);
				break;
			case ShaderParameter::FLOAT:
				spriteRenderer.SpriteShader.SetFloat(parameter.Name, parameter.GetFloatValue());
				break;
			case ShaderParameter::INT:
				spriteRenderer.SpriteShader.SetInt(parameter.Name, parameter.GetIntValue());
				break;
			case ShaderParameter::VEC2:
				glm::vec2 vec2 = parameter.GetVec2();
				spriteRenderer.SpriteShader.SetUniform2f(parameter.Name, vec2.x, vec2.y);
				break;
			case ShaderParameter::VEC3:
				glm::vec3 vec3 = parameter.GetVec3();
				spriteRenderer.SpriteShader.SetUniform3f(parameter.Name, vec3.x, vec3.y, vec3.z);
				break;
			case ShaderParameter::VEC4:
				glm::vec4 vec4 = parameter.GetVec4();
				spriteRenderer.SpriteShader.SetUniform4f(parameter.Name, vec4.x, vec4.y, vec4.z, vec4.w);
				break;
			case ShaderParameter::MAT4:
				spriteRenderer.SpriteShader.SetMatrix4f(parameter.Name, parameter.GetMat4());
				break;
			}
		}
		for (int i = 0; i < spriteRenderer.SpriteTextures.size(); i++)
		{
			spriteRenderer.SpriteTextures[i]->Bind(i);
		}
		spriteRenderer.SpriteShader.SetFloat("xScale", transform.Scale.x);
		spriteRenderer.SpriteShader.SetUniform2f("UVOffset", spriteRenderer.SpriteMaterial->UVOffset.x, spriteRenderer.SpriteMaterial->UVOffset.y);
		spriteRenderer.SpriteShader.SetUniform2f("UVScale", spriteRenderer.SpriteMaterial->UVScale.x, spriteRenderer.SpriteMaterial->UVScale.y);
		spriteRenderer.SpriteShader.SetMatrix4f("model", GetTransformationMatrix(transform));
		_renderer.Draw(spriteRenderer.Quad.Vao, spriteRenderer.Quad.Vbo, spriteRenderer.Quad.Ibo, spriteRenderer.SpriteShader,
						spriteRenderer.SpriteMaterial->GetMaterialType());
	}
	
	_renderer.BeginForwardRendering();
	auto forwardRenderingView = registry.view<SpriteRendererComponent, TransformComponent, ForwardRendererComponent>();
	for (auto entity : forwardRenderingView)
	{
		auto [transform, spriteRenderer] = forwardRenderingView.get<TransformComponent, SpriteRendererComponent>(entity);
		spriteRenderer.SpriteShader.Use();
		auto shaderParams = spriteRenderer.SpriteMaterial->GetShaderParameters();
		for (int i = 0; i < shaderParams.size(); i++)
		{
			auto parameter = shaderParams[i];
			switch (parameter.Type)
			{
			case ShaderParameter::TEXTURE:
				spriteRenderer.SpriteShader.SetUniform1i(parameter.Name, i);
				break;
			case ShaderParameter::FLOAT:
				spriteRenderer.SpriteShader.SetFloat(parameter.Name, parameter.GetFloatValue());
				break;
			case ShaderParameter::INT:
				spriteRenderer.SpriteShader.SetInt(parameter.Name, parameter.GetIntValue());
				break;
			case ShaderParameter::VEC2:
				glm::vec2 vec2 = parameter.GetVec2();
				spriteRenderer.SpriteShader.SetUniform2f(parameter.Name, vec2.x, vec2.y);
				break;
			case ShaderParameter::VEC3:
				glm::vec3 vec3 = parameter.GetVec3();
				spriteRenderer.SpriteShader.SetUniform3f(parameter.Name, vec3.x, vec3.y, vec3.z);
				break;
			case ShaderParameter::VEC4:
				glm::vec4 vec4 = parameter.GetVec4();
				spriteRenderer.SpriteShader.SetUniform4f(parameter.Name, vec4.x, vec4.y, vec4.z, vec4.w);
				break;
			case ShaderParameter::MAT4:
				spriteRenderer.SpriteShader.SetMatrix4f(parameter.Name, parameter.GetMat4());
				break;
			}
		}
		for (int i = 0; i < spriteRenderer.SpriteTextures.size(); i++)
		{
			spriteRenderer.SpriteTextures[i]->Bind(i);
		}
		spriteRenderer.SpriteShader.SetMatrix4f("model", GetTransformationMatrix(transform));
		_renderer.Draw(spriteRenderer.Quad.Vao, spriteRenderer.Quad.Vbo, spriteRenderer.Quad.Ibo, spriteRenderer.SpriteShader,
			spriteRenderer.SpriteMaterial->GetMaterialType());
	}
	_renderer.FinishRendering();
}

glm::mat4 RenderingSystem::GetTransformationMatrix(TransformComponent& transformComponent)
{
	return GetTransformationMatrix(transformComponent.Position, transformComponent.Scale, transformComponent.Rotation);
}

glm::mat4 RenderingSystem::GetTransformationMatrix(glm::vec3 position, glm::vec2 scale, float rotation)
{
	glm::mat4 transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, position);
	transformation = glm::scale(transformation, glm::vec3(scale.x, scale.y, 1.0));
	transformation = glm::rotate(transformation, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
	return transformation;
}

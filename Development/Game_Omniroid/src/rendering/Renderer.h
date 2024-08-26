#pragma once
#include "../debug/DebugLogger.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"

#include "FrameBuffer.h"
#include "materials/Material.h"
#include "GBuffer.h"
#include "ForwardRendererBuffer.h"
#include "postProcessing/PostProcessing.h"
#include "RenderingQuad.h"

#if _DEBUG==1
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x))
#else
#define GLCall(x) x
#endif

void GLClearError();
bool GLLogCall(const char* function);

class Renderer
{
public:
	 Renderer(int renderResolutionX, int renderResolutionY, int screenSize);
	 ~Renderer();
	 bool IsWindowAlive();
	 void BindDefaultFrameBuffer();

	 void BeginDeferredRendering();
	 void RenderDeferredLighting();
	 void BeginForwardRendering();
	 void FinishRendering();
	 void SwapBuffers();

	 void UpdateLight(int idx, glm::vec3 position, glm::vec3 color, float intensity, float sineInfluence, bool castRays);

	 void Draw(VertexArray& vertexArray, VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer,
		 Shader& shader, Material::MaterialType matType);

	 void Draw(VertexArray& vertexArray, VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer);

	 glm::vec2 GetRealScale(int ogWidth, int ogHeight) const;
	 glm::vec2 ConvertScreenToWorldCoords(float ogX, float ogY);

	 glm::vec2 AspectRatio = { 0,0 };
	 int OrthoScale = 0;
	 glm::mat4 ViewMatrix;
	 glm::mat4 ProjectionMatrix;
	 int ScreenSize;
	 glm::ivec2 RenderResolution;
	 GLFWwindow* WindowPtr;

	 inline int GetScreenResolutionX() { return RenderResolution.x * ScreenSize; }
	 inline int GetScreenResolutionY() { return RenderResolution.y * ScreenSize; }

private:

	Shader* _deferredLightingShader;
	RenderingQuad* _deferredLightingQuad;

	Shader* _forwardRenderShader;
	RenderingQuad* _forwardRenderingQuad;

	GBuffer* _gBuffer;
	ForwardRenderBuffer* _forwardRenderBuffer;

	void CreateGLFWWindow();
	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
};
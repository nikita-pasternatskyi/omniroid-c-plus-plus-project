#include "Renderer.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../time/Time.h"


void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function)
{
    while (GLenum error = glGetError())
    {
        LOG_ERROR("[OPENGL]::"<<error<<function);
        return false;
    }
    return true;
}

Renderer::Renderer(int renderResolutionX, int renderResolutionY, int screenSize)
    : ScreenSize(screenSize), RenderResolution(renderResolutionX, renderResolutionY)
{
    CreateGLFWWindow();
    
    _gBuffer = new GBuffer(renderResolutionX, renderResolutionY);
    _deferredLightingShader = new Shader("assets/shaders/deferred_lighting.shader");
    _deferredLightingQuad = new RenderingQuad();

    _deferredLightingShader->Use();
    _deferredLightingShader->SetUniform1i("gAlbedoSpec", 0);
    _deferredLightingShader->SetUniform1i("gParams", 1);
    _deferredLightingShader->SetUniform1i("gNormal", 2);
    _deferredLightingShader->SetUniform1i("gPosition", 3);
    _deferredLightingShader->SetUniform1i("gAdditionalInfo", 4);


    _forwardRenderBuffer = new ForwardRenderBuffer(renderResolutionX, renderResolutionY);
    _forwardRenderShader = new Shader("assets/shaders/forward_renderer.shader");
    _forwardRenderingQuad = new RenderingQuad();

    _forwardRenderShader->Use();
    _forwardRenderShader->SetUniform1i("MainTexture", 0);

    float scaledWidth = AspectRatio.x / OrthoScale;
    float scaledHeight = AspectRatio.y / OrthoScale;
    ProjectionMatrix = glm::ortho(-scaledWidth, scaledWidth, -scaledHeight, scaledHeight, -5.0f, 5.0f);
}

Renderer::~Renderer()
{
    delete _gBuffer;
    delete _deferredLightingShader;
    delete _deferredLightingQuad;
    glfwTerminate();
}

void Renderer::Draw(VertexArray& vertexArray, VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer,
    Shader& shader, Material::MaterialType matType)
{
    shader.Use();
    shader.SetFloat("time", Time::TimeSinceStartup);
    shader.SetMatrix4f("projection", ProjectionMatrix);
    shader.SetMatrix4f("view", ViewMatrix);
    Draw(vertexArray, vertexBuffer, indexBuffer);
}

void Renderer::Draw(VertexArray& vertexArray, VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer)
{
    vertexArray.Bind();
    vertexBuffer.Bind();
    indexBuffer.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}

glm::vec2 Renderer::GetRealScale(int ogWidth, int ogHeight) const
{
    float scaledWidth = AspectRatio.x / OrthoScale;
    float scaledHeight = AspectRatio.y / OrthoScale;
    return glm::vec2((float)ogWidth / RenderResolution.x * scaledWidth, (float)ogHeight / RenderResolution.y * scaledHeight);
}

glm::vec2 Renderer::ConvertScreenToWorldCoords(float ogX, float ogY)
{
    auto screenPos = glm::unProject(glm::vec3(ogX, ogY, 0), glm::mat4(1), ProjectionMatrix, glm::vec4(0, 0, RenderResolution.x, RenderResolution.y));
    screenPos.y = -screenPos.y;
    return screenPos;
}

void Renderer::BeginDeferredRendering()
{
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LESS));
    _gBuffer->Bind();
    GLCall(glViewport(0, 0, RenderResolution.x, RenderResolution.y));
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::RenderDeferredLighting()
{
    _forwardRenderBuffer->Bind();
    GLCall(glDisable(GL_DEPTH_TEST));
    _deferredLightingShader->Use();
    _gBuffer->BindTextures();
    
    auto position = ViewMatrix[3];
    _deferredLightingShader->SetUniform3f("ViewPos", position.x, position.y, position.z);
    
    Draw(_deferredLightingQuad->Vao, _deferredLightingQuad->Vbo, _deferredLightingQuad->Ibo,
        *_deferredLightingShader, Material::MaterialType::SCREEN);
}

void Renderer::BeginForwardRendering()
{
    RenderDeferredLighting();
    _gBuffer->Bind(GL_READ_FRAMEBUFFER);
    _forwardRenderBuffer->Bind(GL_DRAW_FRAMEBUFFER);

    glBlitFramebuffer(0, 0, RenderResolution.x, RenderResolution.y, 0, 0, RenderResolution.x, RenderResolution.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

    _forwardRenderBuffer->Bind();
    GLCall(glEnable(GL_DEPTH_TEST));
}

void Renderer::FinishRendering()
{
    GLCall(glDisable(GL_DEPTH_TEST));
    BindDefaultFrameBuffer();
    GLCall(glViewport(0, 0, GetScreenResolutionX(), GetScreenResolutionY()));
    
    _forwardRenderBuffer->BindTextures();
    _forwardRenderShader->Use();
    Draw(_forwardRenderingQuad->Vao, _forwardRenderingQuad->Vbo, _forwardRenderingQuad->Ibo, 
       *_forwardRenderShader, Material::MaterialType::SCREEN);
}

void Renderer::SwapBuffers()
{
    glfwSwapBuffers(WindowPtr);
    glfwPollEvents();
}

void Renderer::UpdateLight(int idx, glm::vec3 position, glm::vec3 color, float intensity, float sineInfluence, bool castRays)
{
    _deferredLightingShader->Use();
    std::string currentLight = "lights[" + std::to_string(idx) + "].";
    _deferredLightingShader->SetUniform3f(currentLight + "Position", position.x, position.y, position.z);
    _deferredLightingShader->SetUniform3f(currentLight + "Color", color.r, color.g, color.b);
    _deferredLightingShader->SetFloat(currentLight + "Intensity", intensity);
    _deferredLightingShader->SetBool(currentLight + "CastRays", castRays);
    _deferredLightingShader->SetFloat(currentLight + "SineInfluence", sineInfluence);
}

void Renderer::CreateGLFWWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, 0);

    WindowPtr = glfwCreateWindow(GetScreenResolutionX(), GetScreenResolutionY(), "Omniroid", NULL, NULL);
    if (WindowPtr == NULL)
    {
        LOG_ERROR("[GLFW]::Failed to create GLFW window!");
        exit(-1);
    }
    glfwMakeContextCurrent(WindowPtr);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR("[GLAD]::Failed to initialize GLAD!");
        exit(-1);
    }
    GLCall(glViewport(0, 0, GetScreenResolutionX(), GetScreenResolutionY()));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    glfwSetFramebufferSizeCallback(WindowPtr, FrameBufferSizeCallback);
}

void Renderer::BindDefaultFrameBuffer()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

bool Renderer::IsWindowAlive()
{
    return !glfwWindowShouldClose(WindowPtr);
}

void Renderer::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    GLCall(glViewport(0, 0, width, height));
}
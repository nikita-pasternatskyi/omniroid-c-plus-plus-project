#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

struct CameraComponent
{
    CameraComponent(const glm::vec2& aspectRatio, float orthoScale)
        : AspectRatio(aspectRatio), OrthoScale(orthoScale)
    {

    }

    glm::vec2 AspectRatio;
    float OrthoScale;

    glm::mat4 GetProjectionMatrix()
    {
        float scaledWidth = AspectRatio.x / OrthoScale;
        float scaledHeight = AspectRatio.y / OrthoScale;
        return glm::ortho(-scaledWidth, scaledWidth, -scaledHeight, scaledHeight, -5.0f, 5.0f);
    }

};
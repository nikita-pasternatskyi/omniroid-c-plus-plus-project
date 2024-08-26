#pragma once
#include "FrameBuffer.h"
#include <glad/glad.h>

class ForwardRenderBuffer : public FrameBuffer
{
public:
	ForwardRenderBuffer(int width, int height);
};
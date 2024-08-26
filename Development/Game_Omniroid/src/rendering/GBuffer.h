#pragma once
#include "FrameBuffer.h"
class GBuffer : public FrameBuffer
{
public:
	GBuffer(int width, int height);
	static const int ALBEDO = 0;
	static const int ROUGHNESS_METALLIC_EMISSION_AO = 1;
	static const int NORMAL = 2;
	static const int POSITION = 3;
};
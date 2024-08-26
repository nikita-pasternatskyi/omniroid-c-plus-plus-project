#pragma once
#include "../Quad.h"
#include "PostProcessingEffect.h"

class PostProcessing
{
public:
	static void AddPostProcessingEffect(std::shared_ptr<PostProcessingEffect>);
	static void Draw();
private:
	static std::vector<std::shared_ptr<PostProcessingEffect>> _postProcessingEffects;
	static Quad _drawingQuad;
};
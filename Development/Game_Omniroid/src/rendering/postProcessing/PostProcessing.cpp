#include "PostProcessing.h"

std::vector<std::shared_ptr<PostProcessingEffect>>	PostProcessing::_postProcessingEffects;
Quad												PostProcessing::_drawingQuad;

void PostProcessing::AddPostProcessingEffect(std::shared_ptr<PostProcessingEffect> postProcessingEffect)
{
	_postProcessingEffects.push_back(postProcessingEffect);
}

void PostProcessing::Draw()
{
}

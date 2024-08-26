#include "GBuffer.h"
#include "glad/glad.h"
#include <vector>

GBuffer::GBuffer(int width, int height)
	: FrameBuffer(width, height, 5,
		{
		TextureFormat{	GL_RGBA8,	GL_RGBA, GL_UNSIGNED_BYTE},	//albedo
		TextureFormat{	GL_RGBA8,	GL_RGBA, GL_UNSIGNED_BYTE},	//params
		TextureFormat{	GL_RGBA16F,	GL_RGBA, GL_FLOAT},			//normals
		TextureFormat{	GL_RGBA16F,	GL_RGBA, GL_FLOAT},			//pos
		TextureFormat{	GL_RGBA16F,	GL_RGBA, GL_FLOAT},			//additionalInfo - AO + ZBufferPosition + Translucency
		})
{
}
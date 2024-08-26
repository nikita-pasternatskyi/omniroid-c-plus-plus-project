#include "FrameBuffer.h"
#include "Renderer.h"
#include <cstdarg>
#include <cstdio>

FrameBuffer::FrameBuffer(int width, int height)
	: FrameBuffer(width, height, 1, { TextureFormat{GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE} })
{
}

FrameBuffer::FrameBuffer(int width, int height, unsigned int textureCount, std::vector<TextureFormat> format)
{
	GLCall(glGenFramebuffers(1, &_id));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _id));

	std::vector<unsigned int> attachments(textureCount);
	for (int i = 0; i < textureCount; i++)
	{
		TextureFormat texFormat = format[i];
		_textures[i] = std::make_unique<Texture>(width, height, texFormat);
		int attachment = GL_COLOR_ATTACHMENT0 + i;
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, _textures[i]->GetID(), 0));
		attachments[i] = attachment;
	}
	glDrawBuffers(textureCount, attachments.data());
	//create depth and stencil buffer
	GLCall(glGenRenderbuffers(1, &_id));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, _id));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _id));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

	int completeStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (completeStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("[OPENGL]::[FRAMEBUFFER]::Failed to create frame buffer!::" << completeStatus);
	}

	Unbind();
}

FrameBuffer::~FrameBuffer()
{
	GLCall(glDeleteFramebuffers(1, &_id));
}

void FrameBuffer::Bind() const
{
	Bind(GL_FRAMEBUFFER);
}

void FrameBuffer::Bind(int mode) const
{
	GLCall(glBindFramebuffer(mode, _id));
}

void FrameBuffer::Unbind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::BindTextures() const
{
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (_textures[i] == nullptr)
		{
			continue;
		}
		_textures[i]->Bind(i);
	}
}

void FrameBuffer::UnbindTextures() const
{
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (_textures[i] == nullptr)
			continue;
		_textures[i]->Unbind();
	}
}

void FrameBuffer::GetAttachmentParameter()
{
	int ret;
	GLCall(glGetFramebufferAttachmentParameteriv(_id, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &ret));
	LOG(ret);
}
#include "Texture.h"
#include "Renderer.h"

#include "../vendor/stb_image/stb_image.h"

Texture::Texture()
	: _id(0), _localBuffer(nullptr),
	_width(0), _height(0), _bpp(0)
{
}

Texture::Texture(const char* path)
	: _id(0), _localBuffer(nullptr), 
	_width(0), _height(0), _bpp(0)
{
	stbi_set_flip_vertically_on_load(1);
	_localBuffer = stbi_load(path, &_width, &_height, &_bpp, 4);

	if (_localBuffer == NULL)
	{
		LOG_ERROR("[TEXTURE]::Texture under " << path << " was not loaded succesfully!");
	}

	GLCall(glGenTextures(1, &_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, _id));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _localBuffer));
	
	SetGLParameters();

	Unbind();

	if (_localBuffer)
	{
		stbi_image_free(_localBuffer);
	}
}

Texture::Texture(int width, int height)
	: _id(0), _localBuffer(nullptr),
	_width(width), _height(height), _bpp(0)
{
	GLCall(glGenTextures(1, &_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, _id));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _localBuffer));
	SetGLParameters();
	Unbind();
}

Texture::Texture(int width, int height, TextureFormat textureFormat) 
	: _id(0), _localBuffer(nullptr),
	  _width(width), _height(height), _bpp(0)
{
	GLCall(glGenTextures(1, &_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, _id));
	SetGLParameters();
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, textureFormat.InternalFormat, width, height, 0, textureFormat.Format, textureFormat.Type, NULL));
	Unbind();
}


Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &_id));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, _id));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::SetGLParameters()
{
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
}

#pragma once
#include "TextureFormat.h"
class Texture
{
public:
	Texture();
	Texture(const char* path);
	Texture(int width, int height);
	Texture(int width, int height, TextureFormat textureFormat);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return _width; };
	inline int GetHeight() const { return _height; };
	inline unsigned int GetID() const { return _id; }

private:
	void SetGLParameters();
	unsigned int _id;
	unsigned char* _localBuffer;
	int _width, _height, _bpp;
};
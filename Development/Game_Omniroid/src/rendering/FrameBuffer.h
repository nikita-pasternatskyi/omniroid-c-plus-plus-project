#pragma once
#include <memory>
#include <vector>
#include "Texture.h"

class FrameBuffer
{
public:
	FrameBuffer(int width, int height);
	FrameBuffer(int width, int height, unsigned int textureCount, std::vector<TextureFormat> format);
	virtual ~FrameBuffer();
	void Bind() const;
	void Bind(int mode) const;
	void Unbind() const;
	void BindTextures() const;
	void UnbindTextures() const;
	void GetAttachmentParameter();

	static const int MAX_TEXTURES = 16;
private:
	std::unique_ptr<Texture> _textures[MAX_TEXTURES];
	unsigned int _id;
	unsigned int _renderBufferId;
	int _width;
	int _height;
};
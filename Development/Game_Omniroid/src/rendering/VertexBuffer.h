#pragma once
class VertexBuffer
{
private:
	unsigned int _id;
public:
	VertexBuffer(const void* data, unsigned int size);

	void Bind() const;
	void Unbind() const;
};
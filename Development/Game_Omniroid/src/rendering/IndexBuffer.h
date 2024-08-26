#pragma once
class IndexBuffer
{
private:
	unsigned int _id;
	unsigned int _count;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return _count; }
};
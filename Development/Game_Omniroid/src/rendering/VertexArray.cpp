#include "Renderer.h"
#include "VertexArray.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &_id));
	Bind();
}

void VertexArray::AddBuffer(const VertexBufferLayout& layout)
{
	Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for(unsigned int i = 0; i < elements.size(); i++)
	{ 
		const auto& element = elements[i];
		GLCall(glVertexAttribPointer(i, element.Count, element.Type, element.Normalized,
			layout.GetStride(),(const void*)offset));
		GLCall(glEnableVertexAttribArray(i));
		offset += element.Count * VertexBufferElement::GetSizeOfType(element.Type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(_id));

}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

#include "RenderingQuad.h"

RenderingQuad::RenderingQuad()
    : quad(), Vao(), Vbo(quad.Vertices, sizeof(Vertex) * 4), Ibo(quad.Indices, 6)
{
    VertexBufferLayout layout;
    Vertex::PushLayout(layout);
    Vao.AddBuffer(layout);
}

void RenderingQuad::Bind()
{
    Vao.Bind();
    Vbo.Bind();
    Ibo.Bind();
}

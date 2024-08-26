#vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 uv;

out vec2 v_UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    v_UV = uv;
}

#fragment
#version 330 core
out vec4 FragColor;
in vec2 v_UV;

uniform sampler2D AlbedoTexture;
uniform vec4 Tint;

void main()
{
    FragColor = texture2D(AlbedoTexture, v_UV) * Tint;
}
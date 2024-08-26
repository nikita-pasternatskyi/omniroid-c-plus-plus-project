#vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 uv;

out vec2 v_UV;


void main()
{
    gl_Position = vec4(aPos, 1.0);
    v_UV = uv;
}

#fragment
#version 330 core
out vec4 FragColor;
in vec2 v_UV;

uniform sampler2D MainTexture;

void main()
{
    FragColor = texture2D(MainTexture, v_UV);
}
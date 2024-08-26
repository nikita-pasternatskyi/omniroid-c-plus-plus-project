#vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform vec2 UVOffset = vec2(0,0);
uniform vec2 UVScale = vec2(1,1);

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
    
    TexCoords *= UVScale;
    TexCoords += UVOffset;
}

#fragment
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D AlbedoTexture;
uniform vec4 Tint;

uniform vec2 UVOffset = vec2(0,0);
uniform vec2 UVScale = vec2(1,1);
void main()
{
    vec2 c = TexCoords;
    c *= UVScale;
    c += UVOffset;
    FragColor = texture2D(AlbedoTexture, c) * Tint;
}
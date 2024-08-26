#vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec2 UVOffset = vec2(0,0);
uniform vec2 UVScale = vec2(1,1);

void main()
{
    vec4 worldPosition = model * vec4(aPos, 1.0);
    FragPos = worldPosition.xyz;
    TexCoords = aTexCoords;
    TexCoords *= UVScale;
    TexCoords += UVOffset;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#fragment
#version 330 core
layout(location = 0) out vec4 gColor;
layout(location = 1) out vec4 gParams;
layout(location = 2) out vec4 gNormal;
layout(location = 3) out vec4 gPosition;
layout(location = 4) out vec4 gAdditionalInfo;
uniform float xScale;

in vec2 TexCoords;
in vec3 FragPos;

uniform vec4 Tint;
uniform sampler2D AlbedoTexture;
uniform sampler2D ParamsTexture;
uniform sampler2D NormalTexture;
uniform float EmissionMultiplier;
uniform float LightAbsorption = 0;

void main()
{
    gColor = texture2D(AlbedoTexture, TexCoords);
    gColor.rgb *= Tint.rgb;
    if(gColor.a < 1) discard;
    vec3 normal = texture2D(NormalTexture, TexCoords).rgb;
    if(xScale < 0)
    {
        normal.x = 1-normal.x;
    }
    gNormal = vec4(normal, 1.0);
    gParams = texture2D(ParamsTexture, TexCoords);
    gParams.b *= EmissionMultiplier;
    gPosition = vec4(FragPos, 1.0);
    gAdditionalInfo.r = gPosition.z;
    gAdditionalInfo.g = LightAbsorption;
    gAdditionalInfo.b = gColor.a;
    gAdditionalInfo.a = 1;
}
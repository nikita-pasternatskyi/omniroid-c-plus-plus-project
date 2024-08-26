#vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
    gl_Position.z = 0.0;
}

#fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gAlbedoSpec;
uniform sampler2D gParams;
uniform sampler2D gNormal;
uniform sampler2D gPosition;
uniform sampler2D gAdditionalInfo;
uniform mat4 view;
uniform mat4 projection;

struct Light {
    vec3 Position;
    vec3 Color;
    int Type;
    float Radius;
    float Angle;
    float Intensity;
    bool CastRays;
    float SineInfluence;
};
const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 ViewPos;
uniform float time;

const float LIGHT_RAY_STEPS = 480.0;
const float PI = 3.14159265359;

//thanks to learnopengl.com
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec2 WorldToUVSpace(vec3 worldPosition)
{
    vec4 clipSpacePos = projection * (view * vec4(worldPosition + vec3(0,0.1,0), 1.0));
    vec3 ndcSpacePos = clipSpacePos.xyz / clipSpacePos.w;
    return((clipSpacePos.xy + 1.0) / 2.0);
}
const float Epsilon = 1e-10;

vec3 rgb2hsv(in vec3 RGB)
{
    vec4  P   = (RGB.g < RGB.b) ? vec4(RGB.bg, -1.0, 2.0/3.0) : vec4(RGB.gb, 0.0, -1.0/3.0);
    vec4  Q   = (RGB.r < P.x) ? vec4(P.xyw, RGB.r) : vec4(RGB.r, P.yzx);
    float C   = Q.x - min(Q.w, Q.y);
    float H   = abs((Q.w - Q.y) / (6.0 * C + Epsilon) + Q.z);
    vec3  HCV = vec3(H, C, Q.x);
    float S   = HCV.y / (HCV.z + Epsilon);
    return vec3(HCV.x, S, HCV.z);
}

vec3 hsv2rgb(in vec3 HSV)
{
    float H   = HSV.x;
    float R   = abs(H * 6.0 - 3.0) - 1.0;
    float G   = 2.0 - abs(H * 6.0 - 2.0);
    float B   = 2.0 - abs(H * 6.0 - 4.0);
    vec3  RGB = clamp( vec3(R,G,B), 0.0, 1.0 );
    return ((RGB - 1.0) * HSV.y + 1.0) * HSV.z;
}


void main()
{             
    // retrieve data from gbuffer
    vec3 albedo         =   texture2D(gAlbedoSpec, TexCoords).rgb;
    vec3 parameters     =   texture2D(gParams, TexCoords).rgb;
    vec3 normal         =   texture2D(gNormal, TexCoords).rgb;
    vec3 worldPosition  =   texture2D(gPosition, TexCoords).rgb;
    vec3 additionalData =   texture2D(gAdditionalInfo, TexCoords).rgb;
    
    normal = normalize(normal * 2.0 - 1.0);
    float metallic = parameters.r;
    float roughness = parameters.g;
    float emission = parameters.b;
    float sine = abs(sin(time * 1)) + 0.25;

    vec3 N = normalize(normal);
    vec3 V = normalize(ViewPos - worldPosition);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
	           
    // reflectance equation
    vec3 Lo = vec3(0.0);
    vec3 rays = vec3(0.0);
    for(int i = 0; i < NR_LIGHTS; ++i) 
    {
        if(lights[i].Intensity == 0)
        {
            continue;
        }
        
        float lightSine = sine;
        //lightSine -= (1-lights[i].SineInfluence) * lights[i].Intensity;
        // calculate per-light radiance
        vec2 lightUvPosition = WorldToUVSpace(lights[i].Position);
        vec2 UvPosition = WorldToUVSpace(worldPosition);

        vec3 flatLightPosition = vec3(lightUvPosition, 0.0);
        vec3 flatWorldPosition = vec3(UvPosition, 0);//floor(worldPosition.z/4) );

        vec3 L = normalize(flatLightPosition - flatWorldPosition);
        vec3 H = normalize(V + L);
        float distance    = length(flatLightPosition - flatWorldPosition);
        float attenuation = lights[i].Intensity /(distance * distance) * lightSine;
        vec3 radiance     = lights[i].Color * attenuation;        
        
        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);        
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	  
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;  
            
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;

        //light rays

        vec2 difference = TexCoords - lightUvPosition;
        vec2 direction = normalize(difference);
        float uvDistance = length(difference);

        if(direction.y > -0.95 || lights[i].CastRays == false)
        {
           continue;
        }
    
        vec2 delta = direction * (uvDistance/LIGHT_RAY_STEPS);
        vec2 currentCoordinate = lightUvPosition;

        float rayStrength = 1.0;

        vec2 differenceScaled = difference;
        differenceScaled.x *= 1.8;
        float falloff = 1-sqrt(length(differenceScaled * 2));
        rayStrength *= falloff;

        for(int i = 0; i < LIGHT_RAY_STEPS; i++) //go through all pixels horizontally
        {
            float sample = texture2D(gAdditionalInfo, currentCoordinate).r;
            sample = mix(0, clamp(sample, 0, 0.1), 1-floor(sample/4));
            rayStrength *= 1-sample;
            currentCoordinate += delta;
        }
        rayStrength = clamp(rayStrength, 0, 1);
        float mask = 1 - floor(additionalData.r / 4.0);
        mask = clamp(mask, 0,1) * falloff;
        rayStrength += mask * 1;
        rayStrength = clamp(rayStrength, 0, 1);

        rays += vec3(rayStrength) * lights[i].Color * lightSine;
    }   
  
    rays = clamp(rays, 0, 1);
    vec3 mask = 1 - clamp(vec3(floor(additionalData.r / 4.0)), 0, 1);
    rays *= mask;
    rays = clamp(rays, 0, 1);

    vec3 ambient = 0.2 * albedo;
    vec3 color = ambient + Lo;
    
    rays *= sine;
    color = clamp(color, 0, 1) + rays;
    color += emission * albedo;
    FragColor = vec4(color, 1.0);
}
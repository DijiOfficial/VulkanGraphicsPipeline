#version 450

//globals
//const vec3 lightDirection = normalize(vec3(0.0, -1.0, -1.0));
const vec3 lightDirection = normalize(vec3(0.577f, -0.577f, 0.577f));
const vec3 AMBIENT = vec3(0.003f, 0.003f, 0.003f);
//const vec3 AMBIENT = vec3(0.03f, 0.03f, 0.03f);
const float SHININESS = 25.0f;
const float KD = 7.0f;
const float PI = 3.1415927f;

//do I still need fragColor? in 3D meshes since I sample from a default texture anyways
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragTangent;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform sampler2D normalTexSampler;


vec3 MaxToOne(vec3 color)
{
    const float maxValue = max(color.x, max(color.y, color.z));
    return (maxValue > 1.0) ? color / maxValue : color;
}

void main() 
{
    vec3 normal;
    //if (usingNormalMap) 
    //{
    const vec3 binormal = cross(fragNormal, fragTangent);
    const mat3 tangentSpaceAxis = mat3(fragTangent, binormal, fragNormal);
    const vec3 normalMapSample = texture(normalTexSampler, fragTexCoord).rgb;
    const vec3 sampledColor = 2 * normalMapSample - vec3(1, 1, 1);

    normal = tangentSpaceAxis * sampledColor;
    //}
    //else 
    //{
    //    normal = normalize(fragNormal);
    //}

    const float observedArea = dot(normal, -lightDirection);
    if (observedArea < 0) 
    {
        outColor = vec4(0.f, 0.f, 0.f, 1.f);
        return;
    }

    const vec3 colorSample = texture(texSampler, fragTexCoord).rgb;
    const vec3 Lambert = (colorSample * KD) / PI;
    const vec3 BRDF = Lambert + AMBIENT;
    //vec3 BRDF = diffuse + Phong + AMBIENT;

    const vec3 finalColor = MaxToOne(BRDF * observedArea);
    
    outColor = vec4(finalColor, 1.0);
    //outColor = texture(texSampler, fragTexCoord);
}
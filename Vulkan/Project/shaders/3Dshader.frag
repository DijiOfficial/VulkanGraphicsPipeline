#version 450

//globals
#define LAMBERT 0
#define NORMAL 1
#define SPECULAR 2
#define COMBINED 3

const vec3 lightDirection = vec3(0.577f, -0.577f, 0.577f);
const vec3 AMBIENT = vec3(0.03f, 0.03f, 0.03f);
const float SHININESS = 25.0f;
const float KD = 7.f;
const float PI = 3.1415927f;

layout(push_constant) uniform PushConstants{
    vec3 cameraPos;
    int hasNormalMap;
    int shadingMode;
} pushConstants;

layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragTangent;
layout(location = 4) in vec3 fragPosition;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform sampler2D normalSampler;
layout(binding = 3) uniform sampler2D glossSampler;
layout(binding = 4) uniform sampler2D specularSampler;

vec3 MaxToOne(vec3 color)
{
    const float maxValue = max(color.x, max(color.y, color.z));
    return (maxValue > 1.0) ? color / maxValue : color;
}

void main() 
{
    vec3 normal;
    if (pushConstants.hasNormalMap == 1) 
    {
        const vec3 binormal = cross(fragNormal, fragTangent);
        const mat3 tangentSpaceAxis = mat3(fragTangent, binormal, fragNormal);
        const vec3 normalMapSample = texture(normalSampler, fragTexCoord).rgb;
        const vec3 sampledColor = 2 * normalMapSample - vec3(1, 1, 1);

        normal = normalize(tangentSpaceAxis * sampledColor);
    }
    else 
    {
        normal = normalize(fragNormal);
    }

    const float observedArea = dot(normal, -lightDirection);
    if (observedArea < 0) 
    {
        outColor = vec4(0.f, 0.f, 0.f, 1.f);
        return;
    }

    const vec3 colorSample = texture(texSampler, fragTexCoord).rgb;
    const float exp = texture(glossSampler, fragTexCoord).r;
    const vec3 KS = texture(specularSampler, fragTexCoord).rgb;

    const vec3 Lambert = (colorSample * KD) / PI;

    vec3 Phong;
    if (exp == 0.0f) 
	{
        Phong = KS;
    }
    else
    {
        const vec3 viewDirection = normalize(fragPosition - pushConstants.cameraPos);
        const vec3 reflect = reflect(-lightDirection, normal);
	    const float cosAlpha = clamp(dot(reflect, viewDirection), 0.0, 1.0);
        Phong = KS * pow(cosAlpha, exp * SHININESS);
	}
    const vec3 BRDF = Lambert + Phong + AMBIENT;

    vec3 finalColor;
    switch (pushConstants.shadingMode)
    {
        case LAMBERT:
			finalColor = Lambert * observedArea;
			break;
		case NORMAL:
			finalColor = normal;
			break;
		case SPECULAR:
			finalColor = Phong * observedArea;
			break;
		case COMBINED:
			finalColor = BRDF * observedArea;
			break;
    }
    
    outColor = vec4(finalColor, 1.0);
}
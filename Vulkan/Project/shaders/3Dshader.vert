#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec3 inColor;
layout(location = 4) in vec2 inTexCoord;
layout(location = 6) in vec3 inNormal;
layout(location = 8) in vec3 inTangent;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 fragTangent;
layout(location = 4) out vec3 fragPosition;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    fragPosition = gl_Position.xyz;

    const vec4 tNormal =  ubo.model * vec4(inNormal,0);
    fragNormal = normalize(tNormal.xyz); // interpolation of normal attribute in fragment shader.
    
    const vec4 tangent =  ubo.model * vec4(inTangent,0);
    fragTangent = normalize(tangent.xyz);

    fragColor = inColor;
    fragTexCoord = inTexCoord;
}
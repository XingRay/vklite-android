#version 460

// 允许开发者独立编译和管理不同阶段的着色器（如顶点、片段、几何着色器等）
#extension GL_ARB_separate_shader_objects: enable

layout (location = 0) in vec2 inTextureCoordinates;

layout (set = 0, binding = 0) uniform sampler2D textureSampler;

layout (location = 0) out vec4 outColor;

void main() {
    outColor = texture(textureSampler, inTextureCoordinates);
}
#version 460

// 允许开发者独立编译和管理不同阶段的着色器（如顶点、片段、几何着色器等）
#extension GL_ARB_separate_shader_objects: enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTextureCoordinates;

layout (location = 0) out vec2 outTextureCoordinates;

void main() {
    gl_Position = vec4(inPosition, 1.0);
    outTextureCoordinates = inTextureCoordinates;
}
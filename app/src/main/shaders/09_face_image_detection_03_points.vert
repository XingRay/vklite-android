#version 460
layout (location = 0) in vec3 inPosition;

layout (set = 0, binding = 0) uniform PointAttribute {
    vec3 color;
    float size;
} pointAttribute;

layout (location = 0) out vec3 fragColor;

void main() {
    gl_PointSize = pointAttribute.size;
    gl_Position = vec4(inPosition, 1.0);
    fragColor = pointAttribute.color;
}

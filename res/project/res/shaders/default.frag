#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec3 normal;

layout(location = 0) out vec4 outColor;


void main() {
    outColor = vec4(normal, 1.0);
}

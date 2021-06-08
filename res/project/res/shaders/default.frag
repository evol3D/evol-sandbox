#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(1.0, 0.0, 1.0, 1.0);
}

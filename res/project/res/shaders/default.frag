#version 450
#extension GL_EXT_nonuniform_qualifier : require

struct Material {
  vec3 baseColor;
};

layout(location = 0) in vec3 normal;
layout(location = 1) in Material material;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(material.baseColor, 1.0);
}

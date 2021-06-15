#version 450
#extension GL_EXT_nonuniform_qualifier : require

struct Material {
  vec3 baseColor;
};

vec3 directional_light = vec3(-1.0,-1.0,-1.0);

layout(location = 0) in vec3 normal;
layout(location = 1) in Material material;

layout(location = 0) out vec4 outColor;

void main() {
  float intensity = ((dot(normalize(normal), normalize(directional_light)) +1)/2.0)+0.05;

  outColor = vec4(material.baseColor * intensity, 1.0);
}

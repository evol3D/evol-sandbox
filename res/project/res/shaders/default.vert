#version 450
#extension GL_EXT_nonuniform_qualifier : require

struct Vertex {
  vec4 position;
  vec4 normal;
  vec4 color;
  vec2 uv[2];
};

layout(set = 0, binding = 0) buffer ResourceBuffer {
  layout(align = 16) Vertex resources[];
} ResourceBuffers[];

layout(set = 0, binding = 1) buffer PositionBuffer {
  uint resources[];
} PositionBuffers[];

layout(set = 0, binding = 2) buffer ColorBuffer {
  layout(align = 16) vec3 resources[];
} ColorBuffers[];

layout(set = 0, binding = 3) buffer ccBuffer {
  layout(align = 16) vec3 resources[];
} ssBuffers[];

void main() {
	Vertex vertex = ResourceBuffers[ nonuniformEXT(0) ].resources[PositionBuffers[ nonuniformEXT(0) ].resources[gl_VertexIndex] ];


  gl_Position = vec4(vertex.position.x, vertex.position.y, vertex.position[3], 1.0);
}

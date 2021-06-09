#version 450
#extension GL_EXT_nonuniform_qualifier : require

struct Vertex {
  vec4 position;
  vec4 normal;
  vec4 color;
  vec2 uv[2];
};

layout(set = 0, binding = 0) uniform CameraParam {
  mat4 projection;
  mat4 view;
} Camera;

layout(set = 1, binding = 0) buffer ResourceBuffer {
  layout(align = 16) Vertex resources[];
} ResourceBuffers[];

layout(set = 1, binding = 1) buffer PositionBuffer {
  uint resources[];
} PositionBuffers[];

layout(set = 1, binding = 2) buffer ColorBuffer {
  layout(align = 16) vec3 resources[];
} ColorBuffers[];

layout(set = 1, binding = 3) buffer ccBuffer {
  layout(align = 16) vec3 resources[];
} ssBuffers[];

layout(location = 0) out vec3 normal;

void main() {
	Vertex vertex = ResourceBuffers[ nonuniformEXT(0) ].resources[PositionBuffers[ nonuniformEXT(0) ].resources[gl_VertexIndex] ];

  normal = vertex.normal.xyz;
  float scale = 0.3;
  gl_Position = vec4(vertex.position.x * scale - 0.1, vertex.position.y * scale - 0.7, (vertex.position.z * scale * -1.0 + 0.9), 1.0);
}

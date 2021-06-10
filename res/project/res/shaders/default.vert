#version 450
#extension GL_EXT_nonuniform_qualifier : require

struct Vertex {
  vec4 position;
  vec4 normal;
  vec4 color;
  vec2 uv[2];
};

struct Material {
  vec3 baseColor;
};

struct Mesh {
  uint indexBufferIndex;
  uint vertexBufferIndex;
  uint materialBufferIndex;
};

layout( push_constant ) uniform constants
{
	mat4 render_matrix;
  uint meshIndex;
} PushConstants;

layout(set = 0, binding = 0) uniform CameraParam {
  mat4 projection;
  mat4 view;
} Camera;

layout(set = 1, binding = 0) buffer MeshBuffer {
  layout(align = 16) Mesh mesh;
} MeshBuffers[];

layout(set = 1, binding = 1) buffer VertexBuffer {
  layout(align = 16) Vertex vertices[];
} VertexBuffers[];

layout(set = 1, binding = 2) buffer IndexBuffer {
  uint indices[];
} IndexBuffers[];

layout(set = 1, binding = 3) buffer MaterialBuffer {
  layout(align = 16) Material material;
} MaterialBuffers[];

layout(location = 0) out vec3 normal;
layout(location = 1) out Material material;

void main()
{
  Mesh mesh = MeshBuffers[PushConstants.meshIndex].mesh;
  material  = MaterialBuffers[ mesh.materialBufferIndex ].material;

  uint index = IndexBuffers[ mesh.indexBufferIndex ].indices[gl_VertexIndex];
	Vertex vertex = VertexBuffers[ mesh.vertexBufferIndex ].vertices[ index ];

  normal = vertex.normal.xyz;
  float scale = 0.3;
  gl_Position = Camera.projection * Camera.view * PushConstants.render_matrix * vec4(vertex.position.x * scale - 0.1, vertex.position.y * scale - 0.7, (vertex.position.z * scale * -1.0 + 0.9), 1.0);
}

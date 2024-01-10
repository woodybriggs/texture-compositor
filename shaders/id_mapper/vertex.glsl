#version 330;
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out vec2 vert_uv;

void main () {
  gl_Position = vec4(position.x, position.y, position.z, 1.0);
  vert_uv = uv;
}
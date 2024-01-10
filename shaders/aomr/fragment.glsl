#version 330;

in mediump vec2 vert_uv;

uniform sampler2D textures[3];

out mediump vec4 FragColor;

void main() {

  mediump vec4 red = texture(textures[0], vert_uv);
  mediump vec4 green = texture(textures[1], vert_uv);
  mediump vec4 blue = texture(textures[2], vert_uv);

  FragColor = vec4(red.x, green.y, blue.z, 1);
}
#ifndef AOMR_SHADER_H
#define AOMR_SHADER_H

const char aomrVertexSrc[] = 
"#version 330\n"
"layout (location = 0) in vec3 position;"
"layout (location = 1) in vec2 uv;"
""
"out vec2 vert_uv;"
""
"void main () {"
"  gl_Position = vec4(position.x, position.y, position.z, 1.0);"
"  vert_uv = uv;"
"}";

const char aomrFragmentSrc[] =
"#version 330\n"
""
"in mediump vec2 vert_uv;"
""
"uniform sampler2D textures[3];"
""
"out mediump vec4 FragColor;"
""
"void main() {"
""
"  mediump vec4 red = texture(textures[0], vert_uv);"
"  mediump vec4 green = texture(textures[1], vert_uv);"
"  mediump vec4 blue = texture(textures[2], vert_uv);"
""
"  FragColor = vec4(red.x, green.y, blue.z, 1);"
"}";

#endif

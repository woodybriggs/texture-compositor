#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include "shader_program.h"
#include "cstr.h"
#include "texture.h"

typedef struct {
  ShaderProgram shader_program;
  Texture id_map_texture;
  Texture textures[3];
} Compositor;


void compositor_make_texture_surface(Compositor *comp) {

  float surface[] = {
    -1.0, -1.0, 0.0, 0.0, 0.0,
    1.0, -1.0, 0.0, 1.0, 0.0,
    1.0,  1.0, 0.0, 1.0, 1.0,

    -1.0, -1.0, 0.0, 0.0, 0.0,
    -1.0,  1.0, 0.0, 0.0, 1.0, 
    1.0,  1.0, 0.0, 1.0, 1.0,
  };

  GLuint program = comp->shader_program.program;
  glUseProgram(program);

  GLint positionLoc = glGetAttribLocation(program, "position");
  GLint uvLoc = glGetAttribLocation(program, "uv");

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(surface), surface, GL_STATIC_DRAW);
  glAssertNoError();

  glEnableVertexAttribArray(positionLoc);
  GLintptr vertexPositionOffset = 0 * sizeof(float);
  glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (3 + 2), (GLvoid*)vertexPositionOffset);
  glAssertNoError();

  glEnableVertexAttribArray(uvLoc);
  GLintptr vertexUvOffset = 3 * sizeof(float);
  glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * (3 + 2), (GLvoid*)vertexUvOffset);
  glAssertNoError();

  comp->shader_program.vao = vao;
  comp->shader_program.vbo = vbo;
}

Compositor* compoistor_new(const char * vertShaderSrc, const char * fragShaderSrc) {
  Compositor *result = (Compositor*) calloc(1, sizeof(Compositor));

  const char * vertexSources[1] = { vertShaderSrc };
  int vertexSourceCount[1] = { (int) strlen(vertShaderSrc) };
  if (shader_program_make_shader(&result->shader_program, GL_VERTEX_SHADER, vertexSources, vertexSourceCount) < 0) {
    free(result);
    return NULL;
  }

  const char * fragmentSources[1] = { fragShaderSrc };
  int fragmentSourceCount[1] = { (int) strlen(fragShaderSrc) };
  if (shader_program_make_shader(&result->shader_program, GL_FRAGMENT_SHADER, fragmentSources, fragmentSourceCount) < 0) {
    free(result);
    return NULL;
  }

  shader_program_make_program(&result->shader_program);

  compositor_make_texture_surface(result);

  return result;
}

Texture compositor_merge_aomr(Compositor *comp, CStr_Array texture_filenames) {
  Texture result = {0};

  comp->textures[0] = texture_new_rgba_from_filepath(texture_filenames[0]);
  comp->textures[1] = texture_new_rgba_from_filepath(texture_filenames[1]);
  comp->textures[2] = texture_new_rgba_from_filepath(texture_filenames[2]);

  for (int i = 0; i < 3; ++i) {
    if (comp->textures[i].load_result != TextureLoadResult_Success) {
      texture_free(comp->textures[i]);
      fprintf(stderr, "unable to load texture from file %s\n", texture_filenames[i]);
      result.load_result = TextureLoadResult_Failure;
      return result;
    }
  }

  shader_program_upload_textures(&comp->shader_program, &comp->textures[0], "textures");

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindVertexArray(comp->shader_program.vao);
  glBindBuffer(GL_ARRAY_BUFFER, comp->shader_program.vbo);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  GLint dims[4] = {0};
  glGetIntegerv(GL_VIEWPORT, dims);
  int screenWidth = dims[2];
  int screenHeight = dims[3];
  unsigned long long size = screenWidth * screenHeight * 4;
  unsigned char * buffer = (unsigned char *) malloc(size);

  glReadBuffer(GL_BACK);
  glReadPixels(0, 0, screenWidth, screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

  result.width = screenWidth;
  result.height = screenHeight;
  result.num_components = 4;
  result.pixels = buffer;
  result.load_result = TextureLoadResult_Success;

  return result;
}

Texture composite_textures(Compositor *comp, CStr id_map_filename, CStr_Array texture_filenames ) {
  Texture result = {0};
  
  comp->id_map_texture = texture_new_rgba_from_filepath(id_map_filename);
  if (comp->id_map_texture.load_result != TextureLoadResult_Success) {
    fprintf(stderr, "unable to load id_map from file %s\n", id_map_filename);
    result.load_result = TextureLoadResult_Failure;
    return result;
  }

  comp->textures[0] = texture_new_rgba_from_filepath(texture_filenames[0]);
  comp->textures[1] = texture_new_rgba_from_filepath(texture_filenames[1]);
  comp->textures[2] = texture_new_rgba_from_filepath(texture_filenames[2]);

  for (int i = 0; i < 3; ++i) {
    if (comp->textures[i].load_result != TextureLoadResult_Success) {
      texture_free(comp->textures[i]);
      fprintf(stderr, "unable to load texture from file %s\n", texture_filenames[i]);
      result.load_result = TextureLoadResult_Failure;
      return result;
    }
  }

  shader_program_upload_texture(&comp->shader_program, &comp->id_map_texture, "idmap_texture");
  shader_program_upload_textures(&comp->shader_program, &comp->textures[0], "texid_albedo");

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindVertexArray(comp->shader_program.vao);
  glBindBuffer(GL_ARRAY_BUFFER, comp->shader_program.vbo);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  GLint dims[4] = {0};
  glGetIntegerv(GL_VIEWPORT, dims);
  int screenWidth = dims[2];
  int screenHeight = dims[3];
  unsigned long long size = screenWidth * screenHeight * 4;
  unsigned char * buffer = (unsigned char *) malloc(size);

  glReadBuffer(GL_BACK);
  glReadPixels(0, 0, screenWidth, screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

  result.width = screenWidth;
  result.height = screenHeight;
  result.num_components = 4;
  result.pixels = buffer;
  result.load_result = TextureLoadResult_Success;

  return result;
}

void compositor_free_textures(Compositor *comp) {

  unsigned int glTexIDs[3] = { comp->textures[0].glID, comp->textures[1].glID, comp->textures[2].glID }; 
  glDeleteTextures(3, glTexIDs);
  comp->shader_program.consumed_texture_units -=3;

  texture_free(comp->textures[0]);
  texture_free(comp->textures[1]);
  texture_free(comp->textures[2]);
} 

#endif

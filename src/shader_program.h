#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <OpenGL/gl3.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "texture.h"

#define glAssertNoError() (glAssertNoError_impl(__FILE__, __LINE__))

void glAssertNoError_impl(const char * file, unsigned long line) {
  GLenum error = glGetError();
  const char * fmtStr = "glGetError() at %s:%d %s\n";
  switch(error) {
    case GL_INVALID_ENUM: {
      fprintf(stderr, fmtStr, file, line, "GL_INVALID_ENUM");
      exit(1);
    } break;
    case GL_INVALID_VALUE: {
      fprintf(stderr, fmtStr,file, line, "GL_INVALID_VALUE");
      exit(1);
    } break;
    case GL_INVALID_OPERATION: {
      fprintf(stderr, fmtStr,file, line, "GL_INVALID_OPERATION");
      exit(1);
    } break;
    case GL_STACK_OVERFLOW: {
      fprintf(stderr, fmtStr,file, line, "GL_STACK_OVERFLOW");
      exit(1);
    } break;
    case GL_STACK_UNDERFLOW: {
      fprintf(stderr, fmtStr,file, line, "GL_STACK_UNDERFLOW");
      exit(1);
    } break;
    case GL_OUT_OF_MEMORY: {
      fprintf(stderr, fmtStr,file, line, "GL_OUT_OF_MEMORY");
      exit(1);
    } break;
  }
}

void glAssertNoShaderError(GLuint shader) {
  int logLength = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 0) {
    char * buffer = (char *) malloc(logLength+1);
    buffer[logLength+1] = '\0';
    glGetShaderInfoLog(shader, logLength, NULL, buffer);
    fprintf(stderr, "error compiling shader: %s\n", buffer);
    exit(0);
  }
}

typedef struct {

  GLuint vao;
  GLuint vbo;

  GLuint vertexShader;
  const char *const*vertexSources;
  int *vertexSourceCount;

  GLuint fragmentShader;
  const char *const*fragmentSources;
  int *fragmentSourceCount;

  GLuint program;
  int consumed_texture_units;
} ShaderProgram;

int shader_program_make_shader(ShaderProgram *shaderProgram, GLenum GLShaderType, const char * const*sources, int *sourceCount) {
  assert((GLShaderType == GL_VERTEX_SHADER || GLShaderType == GL_FRAGMENT_SHADER) && "unknown shader type");
  GLuint shader = glCreateShader(GLShaderType);
  glShaderSource(shader, 1, sources, sourceCount);
  glCompileShader(shader);
  
  int logLength = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 0) {
    char * logBuffer = (char *) malloc(logLength+1);
    logBuffer[logLength+1] = '\0';
    glGetShaderInfoLog(shader, logLength, NULL, logBuffer);
    fprintf(stderr, "error compiling shader: %s\n", logBuffer);
    free(logBuffer);
    return -1;
  }

  switch (GLShaderType) {
    case GL_VERTEX_SHADER: {
      shaderProgram->vertexShader = shader;
      shaderProgram->vertexSources = sources;
      shaderProgram->vertexSourceCount = sourceCount;
    } break;
    case GL_FRAGMENT_SHADER: {
      shaderProgram->fragmentShader = shader;
      shaderProgram->fragmentSources = sources;
      shaderProgram->fragmentSourceCount = sourceCount;
    } break;
  }

  return shader;
}

int shader_program_make_program(ShaderProgram *shaderProgram) {
  assert(shaderProgram->vertexShader > 0 && "vertex shader is not yet compiled");
  assert(shaderProgram->fragmentShader > 0 && "fragment shader is not yet compiled");

  GLuint program = glCreateProgram();
  glAttachShader(program, shaderProgram->vertexShader);
  glAttachShader(program, shaderProgram->fragmentShader);
  glLinkProgram(program);

  shaderProgram->program = program;

  return program;
}

void shader_program_upload_texture(ShaderProgram *shaderProgram, Texture *texture, const char * uniformName) {
  GLenum texture_unit = GL_TEXTURE0 + shaderProgram->consumed_texture_units;
  GLuint textures[1];
  glGenTextures(1, textures);
  glActiveTexture(texture_unit);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
  glAssertNoError();

  GLuint texture_loc = glGetUniformLocation(shaderProgram->program, uniformName);
  glUniform1i(texture_loc, shaderProgram->consumed_texture_units);
  shaderProgram->consumed_texture_units++;
  texture->glID = textures[0];
}

void shader_program_upload_textures(ShaderProgram *shaderProgram, Texture* textures, const char * uniformName) {
  
  GLuint *glTextures = malloc(sizeof(GLuint) * 3);
  int *textureIndicies = malloc(sizeof(int) * 3);
  glGenTextures(3, glTextures);
  for (int i = 0; i < 3; ++i) {
    GLenum texture_unit = GL_TEXTURE0 + shaderProgram->consumed_texture_units;
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, glTextures[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textures[i].width, textures[i].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[i].pixels);
    textureIndicies[i] = shaderProgram->consumed_texture_units;
    shaderProgram->consumed_texture_units++;
    textures[i].glID = glTextures[i];
  }
  GLuint texture_loc = glGetUniformLocation(shaderProgram->program, uniformName);
  glUniform1iv(texture_loc, 3, textureIndicies);
}
#endif

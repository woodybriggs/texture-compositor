#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "thirdparty/stb/stb_image_write.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

// #include "shaders/id_mapper/id_mapper_shader.h"
#include "shaders/aomr/aomr_shader.h"
#include "shaders/id_mapper/id_mapper_shader.h"
#include "src/shader_program.h"
#include "src/compositor.h"
#include "src/cli-arguments.h"


typedef struct
{
  int size;
  GLFWwindow *window;
} App;

App app;

static void glfwError(int id, const char *description)
{
  fprintf(stderr, "glfw error: %s : %d\n", description, id);
  exit(1);
}

void init_app(App *app, int windowSize) {

  app->size = windowSize;

  glfwSetErrorCallback(&glfwError);
  if (!glfwInit())
  {
    fprintf(stderr, "unable to initalize glfw\n");
    exit(1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  app->window = glfwCreateWindow(app->size, app->size, "Compositor", NULL, NULL);
  if (!app->window)
  {
    fprintf(stderr, "unable to create window\n");
    exit(1);
  }
  glViewport(0, 0, app->size, app->size);
  glfwMakeContextCurrent(app->window);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_MULTISAMPLE);
}


int main(int argc, char ** argv)
{

  CompositeRequest request = cli_args_parse(argc, argv);

  init_app(&app, 1024);

  switch(request.reqType) {
    case CompositeRequestType_Compose: {
      Compositor *comp = compoistor_new(idMapperVertexSrc, idMapperFragmentSrc);
      Texture result = composite_textures(comp, request.id_map_filename, (CStr_Array) {
        request.red_ch_filename,
        request.grn_ch_filename,
        request.blu_ch_filename
      });
      stbi_write_png(request.output_filename, result.width, result.height, 4, result.pixels, result.width * 4);
      texture_free(result);
      compositor_free_textures(comp);
    } break;
    case CompositeRequestType_AOMR: {
      Compositor *comp = compoistor_new(aomrVertexSrc, aomrFragmentSrc);
      Texture result = compositor_merge_aomr(comp, (CStr_Array) {
        request.red_ch_filename,
        request.grn_ch_filename,
        request.blu_ch_filename
      });
      stbi_write_png(request.output_filename, result.width, result.height, 4, result.pixels, result.width * 4);
      texture_free(result);
      compositor_free_textures(comp);
    } break;
    default:
      exit(0);
  }

  glfwTerminate();
  return 0;
}

#ifndef TEXTURE_H
#define TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "../thirdparty/stb/stb_image.h"
#include "cstr.h"

typedef enum {
  TextureLoadResult_Failure = -1,
  TextureLoadResult_Unknown = 0,
  TextureLoadResult_Success = 1
} TextureLoadResult;

typedef struct {
  TextureLoadResult load_result;
  int width;
  int height;
  int num_components;
  unsigned char * pixels;
  unsigned int glID;
} Texture;

Texture texture_new_rgba_from_filepath(CStr filepath) {
  Texture result = {
    .load_result = TextureLoadResult_Unknown,
    .width = 0,
    .height = 0,
    .num_components = 0,
    .pixels = 0
  };
  result.pixels = stbi_load(filepath, &result.width, &result.height, &result.num_components, 4);
  if (result.pixels == NULL) {
    result.load_result = TextureLoadResult_Failure;
    return result;
  }
  result.load_result = TextureLoadResult_Success;
  return result;
}

void texture_free(Texture texture) {
  stbi_image_free(texture.pixels);
}

#endif

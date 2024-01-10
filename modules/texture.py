from enum import Enum


class TextureType(Enum):
  Albedo = 0,
  AmbientOcclusion = 1,
  Metallic = 2,
  Roughness = 3,
  Normal = 4
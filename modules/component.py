
from modules.texture import TextureType


class Component:

  def __init__(self, name) -> None:
    self.name = name

  def get_texture_map_filepath(self, texture_type: TextureType):
    dispatch = {
      TextureType.Albedo: self.get_albedo_map_filepath,
      TextureType.AmbientOcclusion: self.get_ao_map_filepath,
      TextureType.Metallic: self.get_metallic_map_filepath,
      TextureType.Roughness: self.get_roughness_map_filepath,
      TextureType.Normal: self.get_normal_map_filepath
    }
    return dispatch[texture_type]()

  def get_name(self) -> str:
    return self.name

  def get_id_map_filepath(self):
    return f'resources/color-ids/MatID_{self.get_name()}.png'
  
  def get_albedo_map_filepath(self) -> str:
    return f'resources/materials/{self.get_name()}/Legion_{self.get_name()}_BaseColor.png'
  
  def get_ao_map_filepath(self) -> str:
    return f'resources/materials/{self.get_name()}/Legion_{self.get_name()}_AO.png'

  def get_metallic_map_filepath(self) -> str:
    return f'resources/materials/{self.get_name()}/Legion_{self.get_name()}_Metallic.png'

  def get_roughness_map_filepath(self) -> str:
    return f'resources/materials/{self.get_name()}/Legion_{self.get_name()}_Roughness.png'

  def get_normal_map_filepath(self) -> str:
    return f'resources/materials/{self.get_name()}/Legion_{self.get_name()}_Normal.png'
  

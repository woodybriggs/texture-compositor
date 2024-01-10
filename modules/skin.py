
from modules.component import Component
from modules.texture import TextureType


class Skin: 

  def __init__(self, name: str):
    self.name = name

  def get_name(self) -> str:
    return self.name

  def get_texture_map_filepath(self, texture_type: TextureType, component: Component):
    dispatch = {
      TextureType.Albedo: self.get_albedo_map_filepath,
      TextureType.AmbientOcclusion: self.get_ao_map_filepath,
      TextureType.Metallic: self.get_metallic_map_filepath,
      TextureType.Roughness: self.get_roughness_map_filepath,
      TextureType.Normal: self.get_normal_map_filepath
    }
    return dispatch[texture_type](component)
  
  def get_albedo_map_filepath(self, component: Component) -> str:
    pass
  
  def get_ao_map_filepath(self, component: Component) -> str:
    pass

  def get_metallic_map_filepath(self, component: Component) -> str:
    pass

  def get_roughness_map_filepath(self, component: Component) -> str:
    pass

  def get_normal_map_filepath(self, component: Component) -> str:
    pass
  


class BasicSkin(Skin):
  def __init__(self, name: str):
    super().__init__(name)

  def get_albedo_map_filepath(self, component: Component) -> str:
    return f'resources/skins/{self.get_name()}/Legion_{component.get_name()}_BaseColor.png'
  
  def get_ao_map_filepath(self, component: Component) -> str:
    return component.get_ao_map_filepath()

  def get_metallic_map_filepath(self, component: Component) -> str:
    return component.get_metallic_map_filepath()

  def get_roughness_map_filepath(self, component: Component) -> str:
    return component.get_roughness_map_filepath()

  def get_normal_map_filepath(self, component: Component) -> str:
    return component.get_normal_map_filepath()
  

class ComplexSkin(Skin):
  def __init__(self, name: str):
    super().__init__(name)

  def get_albedo_map_filepath(self, component: Component) -> str:
    return f'resources/skins/{self.get_name()}/Legion_{component.get_name()}_BaseColor.png'
  
  def get_ao_map_filepath(self, component: Component) -> str:
    return f'resources/skins/{self.get_name()}/Legion_{component.get_name()}_AO.png'

  def get_metallic_map_filepath(self, component: Component) -> str:
    return f'resources/skins/{self.get_name()}/Legion_{component.get_name()}_Metallic.png'

  def get_roughness_map_filepath(self, component: Component) -> str:
    return f'resources/skins/{self.get_name()}/Legion_{component.get_name()}_Roughness.png'

  def get_normal_map_filepath(self, component: Component) -> str:
    return f'resources/skins/{self.get_name()}/Legion_{component.get_name()}_Normal.png'

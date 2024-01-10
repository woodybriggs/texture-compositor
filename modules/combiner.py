from ast import List
import os
from pathlib import Path
from subprocess import Popen, run
import time

from modules.component import Component
from modules.skin import Skin
from modules.texture import TextureType

class CombinedSkin: 

  def __init__(self, primary: str, secondary: str):
    self.primary = primary
    self.secondary = secondary

  def get_name(self) -> str:
    return f'{self.primary}/{self.secondary}'

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
    return f'generated/{component.get_name()}/{self.get_name()}/{component.get_name()}_Albedo.png'
  
  def get_ao_map_filepath(self, component: Component) -> str:
    return f'generated/{component.get_name()}/{self.get_name()}/{component.get_name()}_AmbientOcclusion.png'

  def get_metallic_map_filepath(self, component: Component) -> str:
    return f'generated/{component.get_name()}/{self.get_name()}/{component.get_name()}_Metallic.png'

  def get_roughness_map_filepath(self, component: Component) -> str:
    return f'generated/{component.get_name()}/{self.get_name()}/{component.get_name()}_Roughness.png'

  def get_normal_map_filepath(self, component: Component) -> str:
    return f'generated/{component.get_name()}/{self.get_name()}/{component.get_name()}_Normal.png'


class Combiner: 

  def __init__(self, component: Component, skin: CombinedSkin):
    self.component: Component = component
    self.skin: CombinedSkin = skin
    return
  

  def get_output_filename(self) -> str:
    output_dir = Path(f'generated/{self.component.get_name()}/{self.skin.get_name()}')
    output_filename = f'{self.component.get_name()}_AOMR.png'
    output = f'{output_dir}/{output_filename}'
    return output


  def combined_aomr_exists(self) -> bool:
    filename = self.get_output_filename()
    return os.path.exists(filename)

  def combine_aomr(self):
    if not self.combined_aomr_exists():
      run([
        "bin/compositor",
        "-type", "aomr",
        "-idmap", "resources/color-ids/MatID_FFFF.png", 
        "-red", self.skin.get_ao_map_filepath(self.component), 
        "-green", self.skin.get_metallic_map_filepath(self.component), 
        "-blue",  self.skin.get_roughness_map_filepath(self.component),
        "-out", self.get_output_filename()
      ])
from ast import List
import os
from pathlib import Path
from subprocess import Popen
import time

from modules.component import Component
from modules.skin import Skin
from modules.texture import TextureType


class Compositor: 

  def __init__(self, component: Component, primary: Skin, secondary: Skin):
    self.component: Component = component
    self.red: Skin = primary
    self.grn: Skin = secondary
    self.running_proccesses: List[Popen[bytes]] = []
    return
  

  def get_output_filename(self, texture_type: TextureType) -> str:
    output_dir = Path(f'generated/{self.component.get_name()}/{self.red.get_name()}/{self.grn.get_name()}')
    output_filename = f'{self.component.get_name()}_{texture_type._name_}.png'
    output_dir.mkdir(exist_ok=True, parents=True)
    output = f'{output_dir}/{output_filename}'
    return output


  def composite_texture_exists(self, texture_type: TextureType) -> bool:
    filename = self.get_output_filename(texture_type)
    return os.path.exists(filename)


  def composite_textures(self):
    for _, texture_type in enumerate(TextureType):
      self.composite_texture(texture_type)


  def composite_texture(self, texture_type: TextureType):
    if not self.composite_texture_exists(texture_type):
      proc = Popen([
        "bin/compositor",
        "-type", "compose",
        "-idmap", self.component.get_id_map_filepath(), 
        "-red", self.red.get_texture_map_filepath(texture_type, self.component), 
        "-green", self.grn.get_texture_map_filepath(texture_type, self.component), 
        "-blue",  self.component.get_texture_map_filepath(texture_type),
        "-out", self.get_output_filename(texture_type)
      ])
      self.running_proccesses.append(proc)
  

  def wait_for_all_processes(self):
    while self.running_proccesses:
      for proc in self.running_proccesses:
        retcode = proc.poll()
        if retcode is not None:
          self.running_proccesses.remove(proc)
        else:
          time.sleep(.01)
          continue
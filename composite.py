from typing import List
from modules.component import Component
from modules.compositor import Compositor
from modules.skin import BasicSkin, ComplexSkin


components: List[Component] = [Component("Helmet01"), Component("Torso"), Component("Arms"), Component("Legs")]


for id, component in enumerate(components):
  compositor = Compositor(component, BasicSkin("White"), ComplexSkin("Silver"))
  compositor.composite_textures();
  compositor.wait_for_all_processes()

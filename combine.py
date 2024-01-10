from typing import List
from modules.combiner import Combiner, CombinedSkin
from modules.component import Component

skin = CombinedSkin("White", "Silver")

components: List[Component] = [Component("Helmet01"), Component("Arms"), Component("Legs"), Component("Torso")]

for comp in components:
  Combiner(comp, skin).combine_aomr()
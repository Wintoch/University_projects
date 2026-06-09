from __future__ import annotations

from python.organism import Plant


class Guarana(Plant):
    def __init__(self, x: int, y: int, game) -> None:
        super().__init__(0, 0, x, y, game)

    def get_color(self) -> tuple[int, int, int]:
        return (255, 105, 180)

    def get_organism(self):
        return self

    def spread(self, target_x: int, target_y: int):
        return Guarana(target_x, target_y, self.game)

    def collision(self, attacker) -> None:
        attacker.set_strength(attacker.get_strength() + 3)
        super().collision(attacker)

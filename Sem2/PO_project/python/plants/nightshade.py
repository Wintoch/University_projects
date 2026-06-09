from __future__ import annotations

from python.organism import Plant


class NightShade(Plant):
    def __init__(self, x: int, y: int, game) -> None:
        super().__init__(99, 0, x, y, game)

    def get_color(self) -> tuple[int, int, int]:
        return (128, 0, 128)

    def get_organism(self):
        return self

    def spread(self, target_x: int, target_y: int):
        return NightShade(target_x, target_y, self.game)

    def collision(self, attacker) -> None:
        self.game.set_cell(attacker.get_x(), attacker.get_y(), None)
        if attacker in self.game.organisms:
            self.game.organisms.remove(attacker)
        super().collision(attacker)

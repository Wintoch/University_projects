from __future__ import annotations

from python.organism import Plant


class Grass(Plant):
    def __init__(self, x: int, y: int, game) -> None:
        super().__init__(0, 0, x, y, game)

    def get_color(self) -> tuple[int, int, int]:
        return (0, 255, 0)

    def ignored_by_predator(self) -> bool:
        return True

    def get_organism(self):
        return self

    def spread(self, target_x: int, target_y: int):
        return Grass(target_x, target_y, self.game)

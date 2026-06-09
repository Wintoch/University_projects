from __future__ import annotations

from python.organism import Animal


class Fox(Animal):
    def __init__(self, strength: int, initiative: int, x: int, y: int, game) -> None:
        super().__init__(strength, initiative, x, y, game)
        self.sense = True

    def get_color(self) -> tuple[int, int, int]:
        return (255, 165, 0)

    def get_organism(self):
        return self

    def reproduce(self, x: int, y: int):
        return Fox(3, 7, x, y, self.game)

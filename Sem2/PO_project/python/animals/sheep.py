from __future__ import annotations

from python.organism import Animal


class Sheep(Animal):
    def __init__(self, strength: int, initiative: int, x: int, y: int, game) -> None:
        super().__init__(strength, initiative, x, y, game)

    def get_color(self) -> tuple[int, int, int]:
        return (255, 255, 255)

    def get_organism(self):
        return self

    def reproduce(self, x: int, y: int):
        return Sheep(4, 4, x, y, self.game)

from __future__ import annotations

from python.organism import Animal


class Turtle(Animal):
    def __init__(self, strength: int, initiative: int, x: int, y: int, game) -> None:
        super().__init__(strength, initiative, x, y, game)

    def get_color(self) -> tuple[int, int, int]:
        return (0, 128, 0)

    def can_deflect(self, attacker) -> bool:
        return attacker.get_strength() < 5

    def get_organism(self):
        return self

    def reproduce(self, x: int, y: int):
        return Turtle(2, 1, x, y, self.game)

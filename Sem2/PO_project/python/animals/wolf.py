from __future__ import annotations

from python.organism import Animal


class Wolf(Animal):
    def __init__(self, strength: int, initiative: int, x: int, y: int, game) -> None:
        super().__init__(strength, initiative, x, y, game)
        self.predator = True

    def get_color(self) -> tuple[int, int, int]:
        return (128, 128, 128)

    def get_organism(self):
        return self

    def reproduce(self, x: int, y: int):
        return Wolf(9, 5, x, y, self.game)

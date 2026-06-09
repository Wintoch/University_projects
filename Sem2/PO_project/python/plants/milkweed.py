from __future__ import annotations

from python.organism import Plant


class Milkweed(Plant):
    def __init__(self, x: int, y: int, game) -> None:
        super().__init__(0, 0, x, y, game)

    def get_color(self) -> tuple[int, int, int]:
        return (255, 255, 0)

    def get_organism(self):
        return self

    def ignored_by_predator(self) -> bool:
        return True

    def action(self) -> None:
        for _ in range(3):
            self.try_to_spread()
        self.age += 1

    def spread(self, target_x: int, target_y: int):
        return Milkweed(target_x, target_y, self.game)

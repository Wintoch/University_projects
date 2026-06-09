from __future__ import annotations

from python.organism import Animal, Plant


class Hogweed(Plant):
    def __init__(self, x: int, y: int, game) -> None:
        super().__init__(10, 0, x, y, game)

    def get_color(self) -> tuple[int, int, int]:
        return (0, 255, 255)

    def get_organism(self):
        return self

    def spread(self, target_x: int, target_y: int):
        return Hogweed(target_x, target_y, self.game)

    def action(self) -> None:
        super().action()

        for dx in range(-1, 2):
            for dy in range(-1, 2):
                if dx == 0 and dy == 0:
                    continue
                nx = self.x + dx
                ny = self.y + dy
                if self.game.is_inside(nx, ny):
                    neighbor = self.game.get_organism_at(nx, ny)
                    if isinstance(neighbor, Animal):
                        self.game.set_cell(nx, ny, None)
                        if neighbor in self.game.organisms:
                            self.game.organisms.remove(neighbor)

    def collision(self, attacker) -> None:
        if attacker in self.game.organisms:
            self.game.organisms.remove(attacker)
        self.game.set_cell(attacker.get_x(), attacker.get_y(), None)
        super().collision(attacker)

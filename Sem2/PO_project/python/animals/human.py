from __future__ import annotations

from python.board import Board
from python.organism import Animal


class Human(Animal):
    BASE_STRENGTH = 5
    SPECIAL_STRENGTH = 10
    SPECIAL_COOLDOWN_TURNS = 5

    def __init__(self, x: int, y: int, game) -> None:
        super().__init__(5, 4, x, y, game)
        self.predator = True
        self.direction = -1
        self.special_active = False
        self.turns_since_activation = -1
        self.cooldownleft = 0

    def get_cooldown_left(self) -> int:
        return self.cooldownleft

    def get_turns_since_activation(self) -> int:
        return self.turns_since_activation

    def restore_special_state(self, strength: int, age: int, active: int, cooldown: int, turns: int) -> None:
        self.strength = strength
        self.age = age
        self.special_active = active == 1
        self.cooldownleft = cooldown
        self.turns_since_activation = turns

    def can_use_special(self) -> bool:
        return not self.special_active and self.cooldownleft == 0

    def activate_special(self) -> bool:
        if self.special_active or self.cooldownleft > 0:
            return False
        self.special_active = True
        self.turns_since_activation = 0
        self.strength = self.SPECIAL_STRENGTH
        return True

    def is_special_active(self) -> bool:
        return self.special_active

    def _update_special_status(self) -> None:
        if self.special_active:
            if self.turns_since_activation > 0:
                self.strength -= 1

            self.turns_since_activation += 1

            if self.strength <= self.BASE_STRENGTH:
                self.strength = self.BASE_STRENGTH
                self.special_active = False
                self.turns_since_activation = -1
                self.cooldownleft = self.SPECIAL_COOLDOWN_TURNS
        elif self.cooldownleft > 0:
            self.cooldownleft -= 1

    def action(self) -> None:
        self._update_special_status()
        self.move()
        self.age += 1

    def get_color(self) -> tuple[int, int, int]:
        return (0, 0, 255)

    def reproduce(self, x: int, y: int):
        return None

    def get_organism(self):
        return self

    def set_direction(self, direction: int) -> None:
        self.direction = direction

    def move(self) -> None:
        from python.plant import Plant

        if self.direction < 0:
            return

        board: Board = self.game.get_board()
        nx, ny = board.step(self.x, self.y, self.direction, 1)

        if not self.game.is_inside(nx, ny):
            self.direction = -1
            return

        other = self.game.get_organism_at(nx, ny)

        if other is None:
            self.leave_cell()
            self.x = nx
            self.y = ny
            self.game.set_cell(self.x, self.y, self)
        elif isinstance(other, Plant):
            if self.is_predator() and other.ignored_by_predator():
                self.step_on_plant(other, nx, ny)
            else:
                self.collision(other)
        else:
            self.collision(other)

        self.direction = -1

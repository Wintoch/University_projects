from __future__ import annotations

from abc import ABC, abstractmethod
from typing import TYPE_CHECKING, Optional

if TYPE_CHECKING:
    from python.game import Game


class Organism(ABC):
    def __init__(self, strength: int, initiative: int, x: int, y: int, game: Game) -> None:
        self.strength = strength
        self.initiative = initiative
        self.age = 0
        self.x = x
        self.y = y
        self.game = game

    @abstractmethod
    def get_color(self) -> tuple[int, int, int]:
        raise NotImplementedError

    @abstractmethod
    def get_organism(self):
        raise NotImplementedError

    @abstractmethod
    def collision(self, other):
        raise NotImplementedError

    @abstractmethod
    def action(self) -> None:
        raise NotImplementedError

    def can_deflect(self, attacker) -> bool:
        return False

    def get_strength(self) -> int:
        return self.strength

    def set_strength(self, strength: int) -> None:
        self.strength = strength

    def get_x(self) -> int:
        return self.x

    def get_y(self) -> int:
        return self.y

    def find_empty_neighbor(self) -> Optional[tuple[int, int]]:
        for nx, ny in self.game.board.get_neighbors(self.x, self.y):
            if self.game.get_organism_at(nx, ny) is None:
                return nx, ny
        return None


class Animal(Organism):
    def __init__(self, strength: int, initiative: int, x: int, y: int, game: Game) -> None:
        super().__init__(strength, initiative, x, y, game)
        self.range = 1
        self.sense = False
        self.predator = False
        self._under_plant = None

    @abstractmethod
    def reproduce(self, x: int, y: int):
        raise NotImplementedError

    def leave_cell(self) -> None:
        if self.game.get_organism_at(self.x, self.y) is not self:
            self._under_plant = None
            return
        if self._under_plant is not None:
            self.game.set_cell(self.x, self.y, self._under_plant)
            self._under_plant = None
        else:
            self.game.set_cell(self.x, self.y, None)

    def step_on_plant(self, plant, nx: int, ny: int) -> None:
        self.leave_cell()
        self._under_plant = plant
        self.x = nx
        self.y = ny
        self.game.set_cell(self.x, self.y, self)

    def is_predator(self) -> bool:
        return self.predator

    def action(self) -> None:
        self.move()
        self.age += 1

    def move(self) -> None:
        import random
        from python.plant import Plant
        from python.animals.turtle import Turtle

        if isinstance(self, Turtle) and random.randrange(100) < 75:
            return

        for _ in range(3):
            board = self.game.board
            direction = random.randrange(board.get_direction_count(self.x))
            nx, ny = board.step(self.x, self.y, direction, self.range)

            if not self.game.is_inside(nx, ny):
                continue

            other = self.game.get_organism_at(nx, ny)
            if other is None:
                self.leave_cell()
                self.x = nx
                self.y = ny
                self.game.set_cell(self.x, self.y, self.get_organism())
                return

            if self.sense and other.strength > self.strength:
                continue

            if other.can_deflect(self):
                continue

            if isinstance(other, Plant) and self.is_predator() and other.ignored_by_predator():
                self.step_on_plant(other, nx, ny)
                return

            self.collision(other)
            return

    def collision(self, other) -> None:
        if self is other:
            return

        if self.__class__ is other.__class__:
            empty = self.find_empty_neighbor()
            if empty is not None:
                self.game.log_event(f"{self.__class__.__name__} rozmnozyl sie")
                offspring = self.reproduce(empty[0], empty[1])
                if offspring is not None:
                    self.game.add_organism_at(empty[0], empty[1], offspring)
            return

        if self.strength >= other.strength:
            kill_eat = " zjadl " if other.__class__.__name__ in {"Grass", "Guarana", "Milkweed", "NightShade", "Hogweed"} else " zabil "
            self.game.log_event(f"{self.__class__.__name__}{kill_eat}{other.__class__.__name__}")
            old_x = self.x
            old_y = self.y
            target_x = other.x
            target_y = other.y

            if other in self.game.organisms:
                self.game.organisms.remove(other)

            self.game.set_cell(old_x, old_y, None)
            self.x = target_x
            self.y = target_y
            self.game.set_cell(target_x, target_y, self)
        else:
            self.game.log_event(f"{other.__class__.__name__} zabil {self.__class__.__name__}")
            self.game.set_cell(self.x, self.y, None)
            if self in self.game.organisms:
                self.game.organisms.remove(self)


class Plant(Organism):
    SPREAD_PROBABILITY = 0.05

    def __init__(self, strength: int, initiative: int, x: int, y: int, game: Game) -> None:
        super().__init__(strength, 0, x, y, game)

    @abstractmethod
    def spread(self, spread_x: int, spread_y: int):
        raise NotImplementedError

    def ignored_by_predator(self) -> bool:
        return False

    def try_to_spread(self) -> None:
        import random

        if random.random() < self.SPREAD_PROBABILITY:
            empty = self.find_empty_neighbor()
            if empty is not None:
                self.game.log_event(f"{self.__class__.__name__} rozprzestrzenil sie")
                offspring = self.spread(empty[0], empty[1])
                self.game.add_organism_at(empty[0], empty[1], offspring)
        self.age += 1

    def action(self) -> None:
        self.try_to_spread()
        self.age += 1

    def collision(self, other) -> None:
        self.game.log_event(f"{other.__class__.__name__} zjadl {self.__class__.__name__}")
        self.game.set_cell(self.x, self.y, None)
        if self in self.game.organisms:
            self.game.organisms.remove(self)

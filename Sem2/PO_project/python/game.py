from __future__ import annotations

from typing import Optional

from python.board import HexBoard, SquareBoard


class Game:
    def __init__(self, rows: int, cols: int) -> None:
        self.rows = rows
        self.cols = cols
        self.cells: list[list[Optional[object]]] = [[None for _ in range(cols)] for _ in range(rows)]
        self.board = SquareBoard(rows, cols)
        self.organisms: list[object] = []
        self.turn_logs: list[str] = []
        self.human = None

    def get_board(self):
        return self.board

    def get_rows(self) -> int:
        return self.rows

    def get_cols(self) -> int:
        return self.cols

    def switch_board(self) -> None:
        if self.board is None or not self.board.is_hexagonal():
            self.board = HexBoard(self.rows, self.cols)
        else:
            self.board = SquareBoard(self.rows, self.cols)
        self.clear_world()
        self.init_world()

    def clear_world(self) -> None:
        for r in range(self.rows):
            for c in range(self.cols):
                self.cells[r][c] = None
        self.organisms.clear()
        self.human = None

    def set_human(self, human) -> None:
        self.human = human

    def begin_turn_logs(self) -> None:
        self.turn_logs.clear()

    def log_event(self, message: str) -> None:
        self.turn_logs.append(message)

    def get_turn_logs(self) -> list[str]:
        return list(self.turn_logs)

    def is_inside(self, row: int, col: int) -> bool:
        return self.board.is_inside(row, col)

    def set_cell(self, r: int, c: int, organism) -> None:
        self.cells[r][c] = organism

    def get_organism_at(self, r: int, c: int):
        if self.is_inside(r, c):
            return self.cells[r][c]
        return None

    def add_organism_at(self, r: int, c: int, org) -> None:
        if self.is_inside(r, c) and self.cells[r][c] is None and org is not None:
            self.cells[r][c] = org
            self.organisms.append(org)

    def activate_human_special(self) -> None:
        if self.human is None:
            return

        if self.human.activate_special():
            self.log_event("Human activated special ability!")
        elif self.human.is_special_active():
            self.log_event("Human's special ability is already active!")
        else:
            self.log_event(f"Human's special ability is on cooldown for {self.human.get_cooldown_left()} more turns.")

    def is_human_special_active(self) -> bool:
        return self.human is not None and self.human.is_special_active()

    def get_human_strength(self) -> int:
        return self.human.get_strength() if self.human is not None else 0

    def get_human_cooldown(self) -> int:
        return self.human.get_cooldown_left() if self.human is not None else 0

    def can_use_ability(self) -> bool:
        return self.human is not None and self.human.can_use_special()

    def get_random_xy(self) -> Optional[tuple[int, int]]:
        import random

        for _ in range(self.rows * self.cols):
            x = random.randrange(self.rows)
            y = random.randrange(self.cols)
            if self.get_organism_at(x, y) is None:
                return x, y
        return None

    def spawn_human_at_center(self) -> None:
        from python.animals.human import Human

        center_x = self.rows // 2
        center_y = self.cols // 2

        if self.cells[center_x][center_y] is not None:
            pos = self.get_random_xy()
            if pos is None:
                return
            center_x, center_y = pos

        human = Human(center_x, center_y, self)
        self.human = human
        self.cells[center_x][center_y] = human
        self.organisms.append(human)

    def create_organism_by_name(self, name: str, r: int, c: int):
        from python.animals.antelope import Antelope
        from python.animals.fox import Fox
        from python.animals.human import Human
        from python.animals.sheep import Sheep
        from python.animals.turtle import Turtle
        from python.animals.wolf import Wolf
        from python.plants.grass import Grass
        from python.plants.guarana import Guarana
        from python.plants.milkweed import Milkweed
        from python.plants.nightshade import NightShade

        if name == "Wolf":
            return Wolf(9, 5, r, c, self)
        if name == "Sheep":
            return Sheep(4, 4, r, c, self)
        if name == "Fox":
            return Fox(3, 7, r, c, self)
        if name == "Turtle":
            return Turtle(2, 1, r, c, self)
        if name == "Antelope":
            return Antelope(4, 4, r, c, self)
        if name == "Grass":
            return Grass(r, c, self)
        if name == "Guarana":
            return Guarana(r, c, self)
        if name == "Milkweed":
            return Milkweed(r, c, self)
        if name == "NightShade":
            return NightShade(r, c, self)
        if name == "Human":
            return Human(r, c, self)
        return None

    def populate_world(self, name: str, count: int) -> None:
        for _ in range(count):
            pos = self.get_random_xy()
            if pos is None:
                return
            new_org = self.create_organism_by_name(name, pos[0], pos[1])
            if new_org is not None:
                self.add_organism_at(pos[0], pos[1], new_org)

    def init_world(self) -> None:
        self.spawn_human_at_center()
        self.populate_world("Wolf", 2)
        self.populate_world("Sheep", 2)
        self.populate_world("Fox", 2)
        self.populate_world("Turtle", 2)
        self.populate_world("Antelope", 2)
        self.populate_world("Grass", 1)
        self.populate_world("Guarana", 1)
        self.populate_world("Milkweed", 1)
        self.populate_world("NightShade", 1)

    def set_human_direction(self, direction: int) -> None:
        if self.human is not None:
            self.human.set_direction(direction)

    def play_turn(self) -> None:
        turn_order = list(self.organisms)
        turn_order.sort(key=lambda org: (org.initiative, org.age), reverse=True)

        for org in turn_order:
            if org not in self.organisms:
                continue
            org.action()

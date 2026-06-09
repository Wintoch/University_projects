from __future__ import annotations

from abc import ABC, abstractmethod


class Board(ABC):
    def __init__(self, rows: int, cols: int) -> None:
        self.rows = rows
        self.cols = cols

    def is_inside(self, row: int, col: int) -> bool:
        return 0 <= row < self.rows and 0 <= col < self.cols

    @abstractmethod
    def get_neighbor_deltas(self, row: int) -> list[tuple[int, int]]:
        raise NotImplementedError

    def get_direction_count(self, row: int) -> int:
        return len(self.get_neighbor_deltas(row))

    def step(self, r: int, c: int, direction: int, steps: int) -> tuple[int, int]:
        cr = r
        cc = c
        for _ in range(steps):
            dr, dc = self.get_neighbor_deltas(cr)[direction]
            cr += dr
            cc += dc
        return cr, cc

    def get_neighbors(self, r: int, c: int) -> list[tuple[int, int]]:
        out: list[tuple[int, int]] = []
        for dr, dc in self.get_neighbor_deltas(r):
            nr = r + dr
            nc = c + dc
            if self.is_inside(nr, nc):
                out.append((nr, nc))
        return out

    @abstractmethod
    def is_hexagonal(self) -> bool:
        raise NotImplementedError


class SquareBoard(Board):
    DELTAS = [
        (-1, -1), (-1, 0), (-1, 1),
        (0, -1),            (0, 1),
        (1, -1),  (1, 0),   (1, 1),
    ]

    def get_neighbor_deltas(self, row: int) -> list[tuple[int, int]]:
        return self.DELTAS

    def is_hexagonal(self) -> bool:
        return False


class HexBoard(Board):
    EVEN = [
        (-1, -1), (-1, 0),
        (0, -1), (0, 1),
        (1, -1), (1, 0),
    ]

    ODD = [
        (-1, 0), (-1, 1),
        (0, -1), (0, 1),
        (1, 0), (1, 1),
    ]

    def get_neighbor_deltas(self, row: int) -> list[tuple[int, int]]:
        return self.EVEN if row % 2 == 0 else self.ODD

    def is_hexagonal(self) -> bool:
        return True

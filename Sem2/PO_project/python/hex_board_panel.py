from __future__ import annotations

import math
from typing import Callable, Optional

import tkinter as tk


class HexBoardPanel(tk.Canvas):
    def __init__(self, master, game, size: int = 16, **kwargs) -> None:
        self.game = game
        self.size = size
        self.on_cell_click: Optional[Callable[[int, int], None]] = None
        self.hexes: dict[tuple[int, int], list[tuple[float, float]]] = {}
        self.padding_x = size
        self.padding_y = size
        self._build_hexes()
        super().__init__(master, background="black", highlightthickness=0, **kwargs)
        self.configure(width=self.width, height=self.height)
        self.bind("<Button-1>", self._handle_click)
        self.redraw()

    def set_on_cell_click(self, handler: Callable[[int, int], None]) -> None:
        self.on_cell_click = handler

    def _build_hex(self, cx: float, cy: float, radius: float) -> list[tuple[float, float]]:
        points: list[tuple[float, float]] = []
        for i in range(6):
            angle = math.radians(60 * i - 30)
            points.append((cx + radius * math.cos(angle), cy + radius * math.sin(angle)))
        return points

    def _build_hexes(self) -> None:
        self.hexes.clear()
        rows = self.game.get_board().rows
        cols = self.game.get_board().cols

        width_step = math.sqrt(3) * self.size
        height_step = 1.5 * self.size
        hex_height = 2 * self.size

        for r in range(rows):
            for c in range(cols):
                cx = self.padding_x + width_step * (c + 0.5 * (r & 1))
                cy = self.padding_y + height_step * r
                self.hexes[(r, c)] = self._build_hex(cx, cy, self.size)

        self.width = int(self.padding_x * 2 + width_step * (cols + 0.5))
        self.height = int(self.padding_y * 2 + height_step * (rows - 1) + hex_height)

    def _color_to_hex(self, color: tuple[int, int, int]) -> str:
        return "#%02x%02x%02x" % color

    def _find_cell(self, x: float, y: float) -> Optional[tuple[int, int]]:
        for cell, poly in self.hexes.items():
            if self._point_in_polygon(x, y, poly):
                return cell
        return None

    @staticmethod
    def _point_in_polygon(x: float, y: float, polygon: list[tuple[float, float]]) -> bool:
        inside = False
        count = len(polygon)
        j = count - 1
        for i in range(count):
            xi, yi = polygon[i]
            xj, yj = polygon[j]
            intersects = ((yi > y) != (yj > y)) and (
                x < (xj - xi) * (y - yi) / ((yj - yi) or 1e-9) + xi
            )
            if intersects:
                inside = not inside
            j = i
        return inside

    def _handle_click(self, event) -> None:
        cell = self._find_cell(event.x, event.y)
        if cell is not None and self.on_cell_click is not None:
            self.on_cell_click(cell[0], cell[1])

    def redraw(self) -> None:
        self.delete("all")
        for (r, c), poly in self.hexes.items():
            org = self.game.get_organism_at(r, c)
            fill = self._color_to_hex(org.get_color()) if org is not None else "#000000"
            points = [coord for point in poly for coord in point]
            self.create_polygon(*points, fill=fill, outline="#282828", tags=("cell", f"cell:{r}:{c}"))

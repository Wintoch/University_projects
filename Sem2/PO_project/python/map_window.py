from __future__ import annotations

import tkinter as tk
from tkinter import messagebox, simpledialog

from python.config import load as load_game
from python.config import save as save_game
from python.game import Game
from python.hex_board_panel import HexBoardPanel


class MapWindow(tk.Tk):
    def __init__(self, rows: int, cols: int) -> None:
        super().__init__()
        self.ROWS = rows
        self.COLS = cols
        self.game = Game(rows, cols)
        self.board_widget = None
        self.square_canvas = None
        self.cell_size = 32

        self._configure_window()
        self._configure_panels()
        self._build_layout()
        self._build_board_view()
        self.game.init_world()
        self._fill_menu()
        self.game.begin_turn_logs()
        self._show_logs(self.game.get_turn_logs())
        self.refresh_board()

        self.bind_all("<Key>", self._on_key_press)
        self.focus_force()

    def _configure_window(self) -> None:
        self.title("Game Map - Jakub Wintoch 208231")
        self.geometry("1000x800")
        self.configure(background="#111111")

    def _configure_panels(self) -> None:
        self.game_board = tk.Frame(self, background="black", highlightbackground="#444444", highlightthickness=1)
        self.menu = tk.Frame(self, background="#1e1e1e", height=180, highlightbackground="#444444", highlightthickness=1)

    def _build_layout(self) -> None:
        self.game_board.pack(fill="both", expand=True)
        self.menu.pack(fill="x", side="bottom")

    def _build_board_view(self) -> None:
        if self.board_widget is not None:
            self.board_widget.destroy()

        if self.game.get_board().is_hexagonal():
            self.board_widget = HexBoardPanel(self.game_board, self.game, size=16)
            self.board_widget.set_on_cell_click(self._on_cell_click)
            self.board_widget.pack(fill="both", expand=True)
        else:
            self.square_canvas = tk.Canvas(self.game_board, background="black", highlightthickness=0)
            self.square_canvas.pack(fill="both", expand=True)
            self.square_canvas.bind("<Button-1>", self._on_square_click)
            self.board_widget = self.square_canvas

        self.game_board.update_idletasks()

    def _color_to_hex(self, color: tuple[int, int, int]) -> str:
        return "#%02x%02x%02x" % color

    def _on_square_click(self, event) -> None:
        row = event.y // self.cell_size
        col = event.x // self.cell_size
        if self.game.is_inside(row, col):
            self._on_cell_click(row, col)

    def _on_cell_click(self, row: int, col: int) -> None:
        if self.game.get_organism_at(row, col) is not None:
            self.game.log_event("This cell is occupied. Cannot add new organism.")
            self._show_logs(self.game.get_turn_logs())
            return

        options = ["Wolf", "Sheep", "Fox", "Turtle", "Antelope", "Grass", "Guarana", "Milkweed", "NightShade"]
        choice = simpledialog.askstring("Add organism", "Choose organism:\n" + ", ".join(options), parent=self)
        if choice is None:
            return

        normalized = choice.strip().lower()
        mapping = {item.lower(): item for item in options}
        choice = mapping.get(normalized)
        if choice is None:
            messagebox.showerror("Invalid choice", "Unknown organism name.")
            return

        org = self.game.create_organism_by_name(choice, row, col)
        if org is not None:
            self.game.add_organism_at(row, col, org)
            self.game.log_event(f"Dodano {choice} na [{row},{col}]")
            self.refresh_board()
            self._show_logs(self.game.get_turn_logs())

    def refresh_board(self) -> None:
        if self.game.get_board().is_hexagonal():
            if isinstance(self.board_widget, HexBoardPanel):
                self.board_widget.redraw()
        else:
            if self.square_canvas is None:
                return
            self.square_canvas.delete("all")
            self.square_canvas.configure(width=self.COLS * self.cell_size, height=self.ROWS * self.cell_size)
            for r in range(self.ROWS):
                for c in range(self.COLS):
                    x1 = c * self.cell_size
                    y1 = r * self.cell_size
                    x2 = x1 + self.cell_size
                    y2 = y1 + self.cell_size
                    org = self.game.get_organism_at(r, c)
                    fill = self._color_to_hex(org.get_color()) if org is not None else "#000000"
                    self.square_canvas.create_rectangle(x1, y1, x2, y2, fill=fill, outline="#282828")

    def _fill_menu(self) -> None:
        for child in self.menu.winfo_children():
            child.destroy()

        legend_panel = tk.Frame(self.menu, background="#1e1e1e")
        legend_panel.pack(side="left", padx=10, pady=10)

        legend_info = [
            ("Human", (0, 0, 255)),
            ("Wolf", (128, 128, 128)),
            ("Sheep", (255, 255, 255)),
            ("Fox", (255, 165, 0)),
            ("Turtle", (0, 128, 0)),
            ("Antelope", (160, 82, 45)),
            ("Grass", (0, 255, 0)),
            ("Guarana", (255, 105, 180)),
            ("Milkweed", (255, 255, 0)),
            ("Nightshade", (128, 0, 128)),
            ("Hogweed", (0, 255, 255)),
        ]

        for name, color in legend_info:
            row = tk.Frame(legend_panel, background="#1e1e1e")
            row.pack(anchor="w")
            box = tk.Frame(row, background=self._color_to_hex(color), width=10, height=10, highlightbackground="#000000", highlightthickness=1)
            box.pack(side="left", padx=(0, 8), pady=2)
            label = tk.Label(row, text=name, foreground="white", background="#1e1e1e")
            label.pack(side="left")

        self.log_area = tk.Text(self.menu, height=8, background="#141414", foreground="white", insertbackground="white", wrap="word", borderwidth=0)
        self.log_area.pack(side="left", fill="both", expand=True, padx=10, pady=10)
        self.log_area.configure(state="disabled")

    def _show_logs(self, logs: list[str]) -> None:
        self._clear_logs()

        special_state = "ON" if self.game.is_human_special_active() else "OFF"
        can_use = "YES" if self.game.can_use_ability() else "NO"
        cooldown = self.game.get_human_cooldown()

        self._add_log(f"Ability: {special_state} | Can use: {can_use} | Cooldown: {cooldown} | Strength Human: {self.game.get_human_strength()}")
        self._add_log("Save to file: S | Load from file: R")
        self._add_log("To switch board press: B")

        for line in logs:
            self._add_log(line)

    def _add_log(self, message: str) -> None:
        self.log_area.configure(state="normal")
        self.log_area.insert("end", message + "\n")
        self.log_area.see("end")
        self.log_area.configure(state="disabled")

    def _clear_logs(self) -> None:
        self.log_area.configure(state="normal")
        self.log_area.delete("1.0", "end")
        self.log_area.configure(state="disabled")

    def _on_key_press(self, event) -> None:
        key = event.keysym
        processed = False

        self.game.begin_turn_logs()

        direction = -1
        is_hex = self.game.get_board().is_hexagonal()

        if not is_hex:
            if key == "Up":
                direction = 1
            elif key == "Down":
                direction = 6
            elif key == "Left":
                direction = 3
            elif key == "Right":
                direction = 4
        else:
            mapping = {"w": 0, "e": 1, "a": 2, "d": 3, "z": 4, "x": 5}
            direction = mapping.get(key.lower(), -1)

        if direction >= 0:
            self.game.set_human_direction(direction)
            processed = True

        lower_key = key.lower()
        if lower_key == "p":
            self.game.activate_human_special()
            processed = True
        elif lower_key == "s":
            self.game.log_event(save_game(self.game))
            self._show_logs(self.game.get_turn_logs())
            return
        elif lower_key == "r":
            self.game.log_event(load_game(self.game))
            self.refresh_board()
            self._show_logs(self.game.get_turn_logs())
            return
        elif lower_key == "b":
            self.game.switch_board()
            self._build_board_view()
            self.refresh_board()
            self._show_logs(self.game.get_turn_logs())
            return

        if processed:
            self.game.play_turn()
            self.refresh_board()
            self._show_logs(self.game.get_turn_logs())

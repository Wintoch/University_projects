from __future__ import annotations

from pathlib import Path

from python.animals.antelope import Antelope
from python.animals.fox import Fox
from python.animals.human import Human
from python.animals.sheep import Sheep
from python.animals.turtle import Turtle
from python.animals.wolf import Wolf
from python.game import Game
from python.plants.grass import Grass
from python.plants.guarana import Guarana
from python.plants.milkweed import Milkweed
from python.plants.nightshade import NightShade

SAVE_PATH = Path(__file__).resolve().parent / "config.txt"


def save(game: Game) -> str:
    try:
        lines: list[str] = []
        board_type = "HEX" if game.get_board().is_hexagonal() else "SQUARE"
        lines.append(f"{board_type} {game.get_rows()} {game.get_cols()}")

        for r in range(game.get_rows()):
            for c in range(game.get_cols()):
                org = game.get_organism_at(r, c)
                if org is None:
                    continue

                type_name = org.__class__.__name__
                strength = org.get_strength()
                age = org.age

                extra1 = 0
                extra2 = 0
                extra3 = 0
                if isinstance(org, Human):
                    extra1 = 1 if org.is_special_active() else 0
                    extra2 = org.get_cooldown_left()
                    extra3 = org.get_turns_since_activation()

                lines.append(f"{type_name} {r} {c} {strength} {age} {extra1} {extra2} {extra3}")

        SAVE_PATH.write_text("\n".join(lines), encoding="utf-8")
        return "Zapisano stan do pliku"
    except Exception as exc:
        return f"Blad zapisu: {exc}"


def load(game: Game) -> str:
    try:
        if not SAVE_PATH.exists() or SAVE_PATH.stat().st_size == 0:
            return "No save file found to load"

        lines = SAVE_PATH.read_text(encoding="utf-8").splitlines()
        if not lines:
            return "No save data found in file"

        header = lines[0].strip().split(" ")
        if len(header) < 3:
            return "Invalid save header"

        saved_type = header[0]
        saved_rows = int(header[1])
        saved_cols = int(header[2])

        current_type = "HEX" if game.get_board().is_hexagonal() else "SQUARE"
        if saved_type != current_type:
            return f"Cannot load: saved board type is {saved_type}, current board type is {current_type}"

        if saved_rows != game.get_rows() or saved_cols != game.get_cols():
            return f"Cannot load: saved size ({saved_rows}x{saved_cols}) differs from current ({game.get_rows()}x{game.get_cols()})"

        game.clear_world()

        for line in lines[1:]:
            text = line.strip()
            if not text:
                continue

            parts = text.split(" ")
            type_name = parts[0]
            r = int(parts[1])
            c = int(parts[2])
            strength = int(parts[3])
            age = int(parts[4])
            extra1 = int(parts[5])
            extra2 = int(parts[6])
            extra3 = int(parts[7])

            org = None
            if type_name == "Wolf":
                org = Wolf(9, 5, r, c, game)
            elif type_name == "Sheep":
                org = Sheep(4, 4, r, c, game)
            elif type_name == "Fox":
                org = Fox(3, 7, r, c, game)
            elif type_name == "Turtle":
                org = Turtle(2, 1, r, c, game)
            elif type_name == "Antelope":
                org = Antelope(4, 4, r, c, game)
            elif type_name == "Grass":
                org = Grass(r, c, game)
            elif type_name == "Guarana":
                org = Guarana(r, c, game)
            elif type_name == "Milkweed":
                org = Milkweed(r, c, game)
            elif type_name == "NightShade":
                org = NightShade(r, c, game)
            elif type_name == "Human":
                human = Human(r, c, game)
                human.restore_special_state(strength, age, extra1, extra2, extra3)
                org = human
                game.set_human(human)

            if org is not None:
                org.strength = strength
                org.age = age
                game.add_organism_at(r, c, org)

        return "Loaded state from file"
    except Exception as exc:
        return f"Load error: {exc}"

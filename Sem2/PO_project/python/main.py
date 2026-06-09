from __future__ import annotations

from python.map_window import MapWindow


def main() -> None:
    window = MapWindow(10, 10)
    window.mainloop()


if __name__ == "__main__":
    main()

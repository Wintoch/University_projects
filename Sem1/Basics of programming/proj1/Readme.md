# Swallow Stars

An event-driven action game implemented in C using the `ncurses` library. Players control a swallow in a high-stakes flight, gathering starlit fireflies while dodging hunters and navigating environmental hazards.

## Gameplay
The objective is to collect a target number of stars before the timer runs out. 
- **Controls**: Use `w/a/s/d` to navigate.
- **Speed Control**: Adjust flight speed with `o` (decrease) and `p` (increase).
- **Survival**: Avoid "hunters" that track your movement. Life force decreases upon collision.
- **Mechanics**: Includes wind gust systems, safe zone taxi services, and procedural level loading.

## Technical Specifications
* **Language**: C (ANSI/ISO compliant)
* **Graphics**: Text-mode via `ncurses`
* **Architecture**: Procedural programming (no OOP/STL).
* **Configuration**: Levels are loaded from external text files (`config.txt`).
* **Game Loop**: Real-time updates with frame-time constraints.

## Project Structure
- `main.c`: Core game loop and state management.
- `characters.c`: Logic for actors (Swallow, Hunters, Stars, Wind).
- `config.c`: Configuration loading and high-score persistence (`stats.txt`).
- `window.c`: UI and status telemetry handling.

## Requirements
* A C compiler (GCC recommended).
* `ncurses` library installed (e.g., `sudo apt install libncurses5-dev` on Linux).

## Compilation
To compile the project, use:
```bash
gcc -o swallow_stars main.c characters.c config.c window.c -lncurses

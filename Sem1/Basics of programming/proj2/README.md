# Beat This Project

A 2D side-scrolling beat'em up game implemented in C++ using the SDL2 library. Players control a brawler, fight various enemies, and navigate through a scrolling level.

## Gameplay
* **Goal**: Defeat all enemies to complete the level.
* **Controls**:
    * `Arrows` / `WASD`: Movement.
    * `Space`: Jump.
    * `J` / `K` / `P`: Different attack actions (Jab, Kick, Punch).
    * `Esc`: Exit to menu.
* **Combo System**: Perform rapid inputs to trigger special actions and increase your score multiplier.
* **Features**: Dynamic health bars, scrolling camera, and an AI-driven enemy system (Chasers and Chargers).

## Technical Specifications
* **Language**: C++
* **Graphics Library**: SDL2 (2.0.3)
* **Architecture**: Object-oriented (Character, Player, Enemy classes).
* **Constraints**: Forbidden: STL containers (`std::vector`, `std::string`, `cin`, `cout`). Standard C libraries (`string.h`, `stdio.h`) are used for file and memory operations.
* **Time Management**: Frame-rate independent gameplay using delta time.

## Project Structure
- `main.cpp`: Entry point, main game loop, and state management.
- `charaters.cpp` / `charaters.h`: Logic for physics, collision detection, AI, and animations.
- `screens.cpp` / `screens.h`: Rendering primitives, text drawing, and UI elements.

## Requirements
* SDL2 development libraries.
* C++ compiler compatible with the laboratory environment.

## Compilation (Linux)
Use the following command for 64-bit systems:
```bash
g++ -O2 -I./sdl/include -L. -o main main.cpp characters.cpp screens.cpp -lSDL2-64 -lpthread -ldl -lrt

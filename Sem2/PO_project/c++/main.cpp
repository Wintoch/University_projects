#include "map.hpp"
#include "game.hpp"
#include <ncurses.h>
#define WIDTH 10
#define HEIGHT 10

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    Game game(WIDTH, HEIGHT);
    game.run();
    return 0;
}
#include "antelope.hpp"

Antelope::Antelope(int x, int y, Map* map)
    : Animal(x, y, 4, 4, map, "Antelope") {}

Antelope* Antelope::reproduce(int newX, int newY) {
    return new Antelope(newX, newY, map);
}

int Antelope::movementRange() const {
    return 2;
}

bool Antelope::tryToEscape(Organism* attacker) {
    if (rand() % 2 != 0) return false;

    int ex = x;
    int ey = y;
    if (map->findFreeSpaceAround(ex, ey)) {
        x = ex;
        y = ey;
        return true;
    }
    return false;
}

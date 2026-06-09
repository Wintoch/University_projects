#include "wolf.hpp"

Wolf::Wolf(int x, int y, Map* map)
    : Animal(x, y, 9, 5, map, "Wolf") {};

Wolf* Wolf::reproduce(int newX, int newY) {
    return new Wolf(newX, newY, map);
}

bool Wolf::canStepOverPlant(const Organism* plant) const {
    return dynamic_cast<const Grass*>(plant) != nullptr ||
           dynamic_cast<const Milkweed*>(plant) != nullptr;
}
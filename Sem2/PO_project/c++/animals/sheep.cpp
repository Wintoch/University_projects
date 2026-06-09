#include "sheep.hpp"

Sheep::Sheep(int x, int y, Map* map)
    : Animal(x, y, 4, 4, map, "Sheep") {};

Sheep* Sheep::reproduce(int newX, int newY) {
    return new Sheep(newX, newY, map);
}
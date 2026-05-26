#include "grass.hpp"

Grass::Grass(int x, int y, Map* map) : Plant(x, y, 0, 0, map) {};

Plant* Grass::reproduce(int newX, int newY) const {
    return new Grass(newX, newY, map);
}
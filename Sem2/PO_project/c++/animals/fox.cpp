#include "fox.hpp"

Fox::Fox(int x, int y, Map* map)
    : Animal(x, y, 3, 7, map, "Fox") {};

Fox* Fox::reproduce(int newX, int newY) {
    return new Fox(newX, newY, map);
};

bool Fox::canEnter(Organism* other) const {
    return (other == nullptr) || (other->getStrength() <= this->getStrength());
}
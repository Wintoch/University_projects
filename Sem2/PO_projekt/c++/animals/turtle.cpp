#include "turtle.hpp"

Turtle::Turtle(int x, int y, Map* map)
    : Animal(x, y, 2, 1, map, "Turtle") {};

Turtle* Turtle::reproduce(int newX, int newY) {
    return new Turtle(newX, newY, map);
}

void Turtle::action() {
    if (rand() % 100 < 75) {
        growOlder();
        return;
    }
    Animal::action();
}

bool Turtle::blocksAttackFrom(Organism* attacker) const {
    return attacker->getStrength() < 5;
}
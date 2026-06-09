#include "plant.hpp"
#include "map.hpp"
#include <cstdlib>

Plant::Plant(int x, int y, int strength, int initiative, Map* map)
    : Organism(x, y, strength, 0, map) {}

void Plant::action() {
    if (rand() % 30 == 0) {
        int newX = x;
        int newY = y;

        if (map->findFreeSpaceAround(newX, newY)) {
            Plant* newPlant = reproduce(newX, newY);
            if (newPlant != nullptr) {
                map->addOrganism(newPlant);
            }
        }
    }
}

void Plant::collision(Organism* other) {
    map->removeOrganism(this);
}
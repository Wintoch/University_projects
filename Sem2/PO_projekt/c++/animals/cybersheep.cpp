#include "cybersheep.hpp"

Cybersheep::Cybersheep(int x, int y, Map* map)
    : Animal(x, y, 10, 4, map, "Cybersheep") {}

Cybersheep* Cybersheep::reproduce(int newX, int newY) {
    return new Cybersheep(newX, newY, map);
}

void Cybersheep::action() {
    // Find nearest nightshade
    int targetX = -1, targetY = -1;
    int minDist = INT_MAX;

    for (int i = 0; i < map->getHeight(); i++) {
        for (int j = 0; j < map->getWidth(); j++) {
            Organism* org = map->getOrganismAt(j, i);
            if (dynamic_cast<Hogweed*>(org)) {
                int dist = abs(x - j) + abs(y - i);
                if (dist < minDist) {
                    minDist = dist;
                    targetX = j;
                    targetY = i;
                }
            }
        }
    }

    if (targetX != -1) {
        // Move towards the target nightshade
        int dx = (targetX > x) ? 1 : (targetX < x) ? -1 : 0;
        int dy = (targetY > y) ? 1 : (targetY < y) ? -1 : 0;

        int newX = x + dx;
        int newY = y + dy;

        if (map->isInside(newX, newY)) {
            Organism* other = map->getOrganismAt(newX, newY);

            if (!canEnter(other)) {
                return;
            }

            if (other != nullptr) {
                Animal* defender = dynamic_cast<Animal*>(other);
                if (defender != nullptr && defender->blocksAttackFrom(this)) {
                    return;
                }
                if (defender != nullptr && defender->tryToEscape(this)) {
                    return;
                }
                this->collision(other);
            } else {
                x = newX;
                y = newY;
            }
        }
    } else {
        // No nightshade found, move randomly
        Animal::action();
    }
}
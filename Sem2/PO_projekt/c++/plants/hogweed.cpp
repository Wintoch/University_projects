#include "hogweed.hpp"

Hogweed::Hogweed(int x, int y, Map* map) : Plant(x, y, 10, 0, map) {}

void Hogweed::action() {
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            int nx = x + dx;
            int ny = y + dy;
            if (!map->isInside(nx, ny)) continue;

            Organism* other = map->getOrganismAt(nx, ny);
            if (other == nullptr) continue;

            //Cybersheep is immune to Hogweed's effect
            if (dynamic_cast<Cybersheep*>(other) != nullptr) {
                continue;
            }

            // Remove the organism and log the event
            std::string victimName = map->getOrganismName(other);
            map->removeOrganism(other);
            map->logEvent(victimName + " zostal zabity przez Barszcz Sosnowskiego");
        }
    }

    Plant::action();
}

Plant* Hogweed::reproduce(int newX, int newY) const {
    return new Hogweed(newX, newY, map);
}
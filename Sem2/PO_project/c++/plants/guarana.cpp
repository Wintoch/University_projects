#include "guarana.hpp"

Gurana::Gurana(int x, int y, Map* map) : Plant(x, y, 0, 0, map) {};

void Gurana::collision(Organism* other) {
    if (other->getStrength() > 0) {
        other->setStrength(other->getStrength() + 3);
        map->logEvent(map->getOrganismName(other) + " zjadl Guarane (+3 sily)!");
    }
    Plant::collision(other);
}

Plant* Gurana::reproduce(int newX, int newY) const {
    return new Gurana(newX, newY, map);
}
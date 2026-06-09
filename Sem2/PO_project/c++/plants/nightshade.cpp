#include "nightshade.hpp"

Nightshade::Nightshade(int x, int y, Map* map) : Plant(x, y, 99, 0, map) {};

void Nightshade::collision(Organism* other) {
    std::string eaterName = map->getOrganismName(other); // najpierw zapisz nazwe
    map->removeOrganism(other);
    map->removeOrganism(this);
    map->logEvent(eaterName + " zjadl wilcze jagody i umarl!");
}

Plant* Nightshade::reproduce(int newX, int newY) const {
    return new Nightshade(newX, newY, map);
}
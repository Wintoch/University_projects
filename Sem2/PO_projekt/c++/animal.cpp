#include "animal.hpp"
#include "map.hpp"
#include <cstdlib>
#include <algorithm>

Animal::Animal(int x, int y, int strength, int initiative, Map* map, string species)
    : Organism(x, y, strength, initiative, map), species(species) {}

void Animal::action() {
    move();
    growOlder();
}

bool Animal::canEnter(Organism* other) const {
    return true;
}

bool Animal::blocksAttackFrom(Organism* attacker) const {
    return false;
}

bool Animal::tryToEscape(Organism* attacker) {
    return false;
}

int Animal::movementRange() const {
    return 1;
}

void Animal::move() {
    int dx = 0, dy = 0;

    do{
        dx = rand() % 3 - 1;
        dy = rand() % 3 - 1;
    }while(dx == 0 && dy == 0);

    int step = movementRange();
    int newX = this->x + dx * step;
    int newY = this->y + dy * step;

    if(!map->isInside(newX, newY)) return;

    Organism* otherAnimal = map->getAnimalAt(newX, newY);
    if (otherAnimal != nullptr && otherAnimal != this) {
        Animal* defender = dynamic_cast<Animal*>(otherAnimal);
        if (defender != nullptr && defender->blocksAttackFrom(this)) {
            return;
        }
        if (defender != nullptr && defender->tryToEscape(this)) {
            return;
        }
        collision(otherAnimal);
        return;
    }

    Organism* otherPlant = map->getPlantAt(newX, newY);
    if (otherPlant != nullptr) {
        if (canStepOverPlant(otherPlant)) {
            x = newX;
            y = newY;
        } else {
            collision(otherPlant);
        }
        return;
    }

    x = newX;
    y = newY;
}


void Animal::collision(Organism* other) {
    Plant* plant = dynamic_cast<Plant*>(other);
    if (plant != nullptr) {
        other->collision(this);
        return;
    }

    Animal* otherAnimal = dynamic_cast<Animal*>(other);
    if (otherAnimal != nullptr && this->species == otherAnimal->species) {
        int babyX = this->x;
        int babyY = this->y;

        if (map->findFreeSpaceAround(babyX, babyY)) {
            Organism* baby = this->reproduce(babyX, babyY);
            if (baby != nullptr) {
                map->addOrganism(baby);
                map->logEvent(map->getOrganismName(this) + " rozmnozyl sie!");
            }
        }
    } else {
        const std::string attackerName = map->getOrganismName(this);
        const std::string defenderName = map->getOrganismName(other);

        if(this->strength >= other->getStrength()) {
            map->logEvent(attackerName + " zabil " + defenderName);
            map->removeOrganism(other);
        } else {
            map->logEvent(defenderName + " zabil " + attackerName);
            map->removeOrganism(this);
        }
    }
}

bool Animal::canStepOverPlant(const Organism* plant) const {
    return false;
}
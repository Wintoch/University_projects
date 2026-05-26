#include "human.hpp"
#include <vector>

Human::Human(int x, int y, Map* map)
    : Animal(x, y, 5, 4, map, "Human"), pendingDx(0), pendingDy(0),
      abilityActive(false), abilityTurnsLeft(0), cooldownTurnsLeft(0), 
      abilityRequested(false), elixirStrength(5) {}

void Human::setDirection(int dx, int dy) {
    pendingDx = dx;
    pendingDy = dy;
}

void Human::setPendingDirection(int dx, int dy) {
    pendingDx = dx;
    pendingDy = dy;
}

void Human::setAbilityState(bool active, int turnsLeft, int cooldownLeft, bool requested) {
    abilityActive = active;
    abilityTurnsLeft = turnsLeft;
    cooldownTurnsLeft = cooldownLeft;
    abilityRequested = requested;
}

void Human::requestAbilityActivation() {
    abilityRequested = true;
}

void Human::tickAbility() {
    bool justActivated = false;

    if (abilityRequested && !abilityActive && cooldownTurnsLeft == 0) {
        abilityActive = true;
        abilityTurnsLeft = 5;
        elixirStrength = 10;
        justActivated = true;
        map->logEvent("Human aktywowal Magiczny Eliksir");
    }
    abilityRequested = false;

    if (abilityActive) {
        if (abilityTurnsLeft == 0) {
            abilityActive = false;
            cooldownTurnsLeft = 5;
            elixirStrength = strength;
            map->logEvent("Magiczny Eliksir wygasl");
        } else {
            if (!justActivated && elixirStrength > strength) {
                elixirStrength--;
            }
            abilityTurnsLeft--;
        }
    } else if (cooldownTurnsLeft > 0) {
        cooldownTurnsLeft--;
    }
}

void Human::action() {
    tickAbility();
    move();
    growOlder();
}

void Human::move() {
    if (pendingDx == 0 && pendingDy == 0) return;

    int newX = x + pendingDx;
    int newY = y + pendingDy;

    pendingDx = 0;
    pendingDy = 0;

    if (!map->isInside(newX, newY)) return;

    Organism* otherAnimal = map->getAnimalAt(newX, newY);
    if (otherAnimal != nullptr && otherAnimal != this) {
        Animal* defender = dynamic_cast<Animal*>(otherAnimal);
        if (defender != nullptr && defender->blocksAttackFrom(this)) return;
        if (defender != nullptr && defender->tryToEscape(this)) return;
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

void Human::collision(Organism* other) {
    Plant* plant = dynamic_cast<Plant*>(other);
    if (plant != nullptr) {
        other->collision(this);
        return;
    }

    const std::string attackerName = map->getOrganismName(this);
    const std::string defenderName = map->getOrganismName(other);

    int effectiveStrength = abilityActive ? elixirStrength : strength;
    if (effectiveStrength >= other->getStrength()) {
        map->logEvent(attackerName + " zabil " + defenderName);
        map->removeOrganism(other);
    } else {
        map->logEvent(defenderName + " zabil " + attackerName);
        map->removeOrganism(this);
    }
}

Human* Human::reproduce(int newX, int newY) {
    return nullptr;
}

bool Human::canStepOverPlant(const Organism* plant) const {
    return dynamic_cast<const Grass*>(plant) != nullptr ||
           dynamic_cast<const Milkweed*>(plant) != nullptr;
}
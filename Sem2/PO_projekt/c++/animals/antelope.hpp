#pragma once 
#include "../animal.hpp"
#include "../map.hpp"

class Antelope : public Animal {
    protected:
        bool tryToEscape(Organism* attacker) override;
        int movementRange() const override;
    public:
        Antelope(int x, int y, Map* map);
        Antelope* reproduce(int newX, int newY) override;
};
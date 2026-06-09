#pragma once
#include "../animal.hpp"
#include "../map.hpp"

class Fox : public Animal {
    protected:
        bool canEnter(Organism* other) const override;
    public:
        Fox(int x, int y, Map* map);
        Fox* reproduce(int newX, int newY) override;
};
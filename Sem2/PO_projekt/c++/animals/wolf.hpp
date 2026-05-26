#pragma once
#include "../animal.hpp"
#include "../plants/grass.hpp"
#include "../plants/milkweed.hpp"

class Wolf : public Animal {
    public:
        Wolf(int x, int y, Map* map);
        Wolf* reproduce(int newX, int newY) override;
        bool canStepOverPlant(const Organism* plant) const override;
};
#pragma once
#include "../plant.hpp"
#include "../map.hpp"

class Nightshade : public Plant {
    public:
        Nightshade(int x, int y, Map* map);
        void collision(Organism* other) override;
        Plant* reproduce(int newX, int newY) const override;
};
#pragma once
#include "../plant.hpp"
#include "../map.hpp"

class Gurana : public Plant {
    public:
        Gurana(int x, int y, Map* map);
        void collision(Organism* other) override;
        Plant* reproduce(int newX, int newY) const override;
};
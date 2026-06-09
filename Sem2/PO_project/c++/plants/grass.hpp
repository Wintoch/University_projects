#pragma once
#include "../plant.hpp"

class Grass : public Plant {
    public:
        Grass(int x, int y, Map* map);
        Plant* reproduce(int newX, int newY) const override;
};
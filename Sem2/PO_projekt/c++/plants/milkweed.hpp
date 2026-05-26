#pragma once
#include "../plant.hpp"

class Milkweed : public Plant {
    public:
        Milkweed(int x, int y, Map* map);
        void action() override;
        Plant* reproduce(int newX, int newY) const override;
};
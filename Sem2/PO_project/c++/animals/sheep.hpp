#pragma once
#include "../animal.hpp"

class Sheep : public Animal {
    public:
        Sheep(int x, int y, Map* map);
        Sheep* reproduce(int newX, int newY) override;
};
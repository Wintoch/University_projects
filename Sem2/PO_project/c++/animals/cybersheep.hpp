#pragma once
#include "../animal.hpp"
#include "../map.hpp"

class Cybersheep : public Animal {
    public:
        Cybersheep(int x, int y, Map* map);
        Cybersheep* reproduce(int newX, int newY) override;
        void action() override;
};
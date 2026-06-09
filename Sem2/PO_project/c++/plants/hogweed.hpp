#pragma once
#include "../plant.hpp"
#include "../map.hpp"

class Hogweed : public Plant {
    public:
        Hogweed(int x, int y, Map* map);
        void action() override;
        Plant* reproduce(int newX, int newY) const override;
};
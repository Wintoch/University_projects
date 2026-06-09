#pragma once
#include "organism.hpp"

class Plant : public Organism{
    public:
        Plant(int x, int y, int strength, int initiative, Map* map);
        void action() override;
        void collision(Organism* other) override;
        void move() override {};
        virtual Plant* reproduce(int newX, int newY) const = 0;
};
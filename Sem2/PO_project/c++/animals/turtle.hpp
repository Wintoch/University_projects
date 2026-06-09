#pragma once
#include "../animal.hpp"

class Turtle : public Animal {
    protected:
        virtual bool blocksAttackFrom(Organism* attacker) const override;
    public:
        Turtle(int x, int y, Map* map);
        virtual void action() override;
        Turtle* reproduce(int newX, int newY) override;
};
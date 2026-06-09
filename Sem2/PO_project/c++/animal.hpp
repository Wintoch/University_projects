#pragma once
#include "organism.hpp"
#include <string>
using namespace std;

class Animal : public Organism {
    private:
        string species;
    protected:
        virtual bool canEnter(Organism* other) const;
        virtual int movementRange() const;
        virtual bool canStepOverPlant(const Organism* plant) const;
    public:
        Animal(int x, int y, int strength, int initiative, Map* map, string species);
        void action() override;
        void collision(Organism* other) override;
        void move() override;
        virtual Animal* reproduce(int newX, int newY) = 0;
        virtual bool blocksAttackFrom(Organism* attacker) const;
        virtual bool tryToEscape(Organism* attacker);
};
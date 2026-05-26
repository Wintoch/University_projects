#pragma once
#include "../animal.hpp"
#include "../map.hpp"

class Human : public Animal {
    private:
        int pendingDx;
        int pendingDy;

        bool abilityActive;
        int abilityTurnsLeft;
        int cooldownTurnsLeft;
        bool abilityRequested;

        void tickAbility();
        int elixirStrength;
    public:
        Human(int x, int y, Map* map);

        void action() override;
        void collision(Organism* other) override;
        void move() override;
        Human* reproduce(int newX, int newY) override;

        void setDirection(int dx, int dy);
        void requestAbilityActivation();

        bool isAbilityActive() const { return abilityActive; }
        bool canActivateAbility() const { return !abilityActive && cooldownTurnsLeft == 0; }
        int getAbilityTurnsLeft() const { return abilityTurnsLeft; }
        int getCooldownTurnsLeft() const { return cooldownTurnsLeft; }

        bool canStepOverPlant(const Organism* plant) const override;

        int getPendingDx() const { return pendingDx; }
        int getPendingDy() const { return pendingDy; }
        bool isAbilityRequested() const { return abilityRequested; }

        void setPendingDirection(int dx, int dy);
        void setAbilityState(bool active, int turnsLeft, int cooldownLeft, bool requested);

};
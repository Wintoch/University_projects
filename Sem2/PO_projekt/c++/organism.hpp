#pragma once

class Map;
class Organism{
    protected:
        int x, y;
        int strength;
        int initiative;
        Map* map;
        int age;
    public:
        Organism(int x, int y, int strength, int initiative, Map* map):
            x(x), y(y), strength(strength), initiative(initiative), map(map), age(0) {};
        int getX() const { return x; }
        int getY() const { return y; }
        virtual ~Organism() = default;
        virtual void action() = 0;
        virtual void collision(Organism* other) = 0;
        virtual void move() = 0;
        int getStrength() const { return strength; }
        void setStrength(int newStrength) { strength = newStrength; }
        int getInitiative() const { return initiative; }
        int getAge() const { return age; }
        void growOlder() { age++; }

        void setInitiative(int newInitiative) { initiative = newInitiative; }
        void setAge(int newAge) { age = newAge; }
        void setPosition(int newX, int newY) { x = newX; y = newY; }
};
#pragma once
#include <ncurses.h>
#include <vector>
#include <deque>
#include "animals/wolf.hpp"
#include "animals/sheep.hpp"
#include "animals/fox.hpp"
#include "animals/turtle.hpp"
#include "animals/antelope.hpp"
#include "animals/cybersheep.hpp"
#include "animals/human.hpp"
#include "plants/grass.hpp"
#include "plants/milkweed.hpp"
#include "plants/guarana.hpp"
#include "plants/nightshade.hpp"
#include "plants/hogweed.hpp"
#include "organism.hpp"

class Organism;

class Map {
    private :
        int width, height;
        std::vector<std::vector<char>> grid;
        std::vector<Organism*> organisms;
        std::deque<std::string> events;
        std::vector<Organism*> toDelete;
    public :
        Map(int width, int height);
        void generate();
        void draw(WINDOW* win = stdscr) const;
        int getWidth() const { return width; }
        int getHeight() const { return height; }
        bool findFreeSpaceAround(int&x, int& y);
        void addOrganism(Organism* organism);
        void removeOrganism(Organism* organism);
        void playTurn();
        bool isInside(int x, int y) const;
        Organism* getOrganismAt(int x, int y) const;
        bool getRandomFreePosition(int& x, int& y) const;
        short getColorPairFor(const Organism* org) const;
        void logEvent(const std::string& msg);
        const std::deque<std::string>& getEvents() const;
        void clearEvents();
        std::string getOrganismName(const Organism* org) const;
        Organism* getAnimalAt(int x, int y) const;
        Organism* getPlantAt(int x, int y) const;
        const std::vector<Organism*>& getOrganisms() const;
        void clearWorld();
};
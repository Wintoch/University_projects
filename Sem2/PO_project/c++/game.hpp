#pragma once
#include <ncurses.h>
#include "map.hpp"
#include "animals/human.hpp"
#include "animals/wolf.hpp"
#include "animals/sheep.hpp"
#include "animals/fox.hpp"
#include "animals/turtle.hpp"
#include "animals/antelope.hpp"
#include "animals/cybersheep.hpp"
#include "plants/grass.hpp"
#include "plants/milkweed.hpp"
#include "plants/guarana.hpp"
#include "plants/nightshade.hpp"
#include "plants/hogweed.hpp"
#include <string>

class Game {
private:
    WINDOW* win;
    WINDOW* infoWin;
    Map map;
    int width, height;
    Human* human;

    std::string getTypeToken(const Organism* org) const;
    Organism* createOrganismByType(const std::string& type, int x, int y);
    
    void initNcurses();
    void drawInfoPanel();
    
public:
    bool saveToFile(const std::string& path) const;
    bool loadFromFile(const std::string& path);
    Game(int width, int height);
    ~Game();
    void run();
};
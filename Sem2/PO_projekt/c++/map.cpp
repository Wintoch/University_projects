#include "map.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>

Map::Map(int width, int height) : width(width), height(height) {
    grid.resize(height, std::vector<char>(width, ' '));
}

void Map::generate() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x] = ' ';
        }
    }
}

short Map::getColorPairFor(const Organism* org) const {
    if (dynamic_cast<const Wolf*>(org)) return 1; // wolf: gray square
    if (dynamic_cast<const Sheep*>(org)) return 2; // sheep: white square
    if (dynamic_cast<const Fox*>(org)) return 3;// fox: orange square
    if (dynamic_cast<const Turtle*>(org)) return 4;// turtle: green square
    if (dynamic_cast<const Antelope*>(org)) return 5;// antelope: brown square
    if (dynamic_cast<const Cybersheep*>(org)) return 6;// human: cyan square
    if (dynamic_cast<const Grass*>(org)) return 7;// grass: green square
    if (dynamic_cast<const Milkweed*>(org)) return 8;// milkweed: yellow square
    if (dynamic_cast<const Gurana*>(org)) return 9;// guarana: red square
    if (dynamic_cast<const Nightshade*>(org)) return 10;// nightshade: magenta square
    if (dynamic_cast<const Hogweed*>(org)) return 11;// hogweed: blue square
    return 0;
}

void Map::draw(WINDOW* win) const {
    box(win, 0, 0);

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            mvwaddch(win, y, x, ' ');
        }
    }

    auto drawOne = [&](Organism* org) {
        if (org == nullptr) return;

        int ox = org->getX();
        int oy = org->getY();

        if (ox <= 0 || ox >= width - 1 || oy <= 0 || oy >= height - 1) return;

        short cp = getColorPairFor(org);
        if (cp != 0) {
            if (cp == 1) wattron(win, COLOR_PAIR(cp) | A_DIM);
            else wattron(win, COLOR_PAIR(cp));

            mvwaddch(win, oy, ox, ACS_CKBOARD);

            if (cp == 1) wattroff(win, COLOR_PAIR(cp) | A_DIM);
            else wattroff(win, COLOR_PAIR(cp));
        } else {
            mvwaddch(win, oy, ox, 'O');
        }
    };

    for (Organism* org : organisms) {
        if (dynamic_cast<Plant*>(org) != nullptr) {
            drawOne(org);
        }
    }

    for (Organism* org : organisms) {
        if (dynamic_cast<Animal*>(org) != nullptr) {
            drawOne(org);
        }
    }

    wrefresh(win);
    refresh();
}

bool Map::findFreeSpaceAround(int& x, int& y) {
    std::vector<std::pair<int, int>> freeSpaces;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            int newX = x + dx;
            int newY = y + dy;
            if (isInside(newX, newY) && getOrganismAt(newX, newY) == nullptr) {
                freeSpaces.emplace_back(newX, newY);
            }
        }
    }

    if (freeSpaces.empty()) return false;

    int randomIndex = rand() % freeSpaces.size();

    x = freeSpaces[randomIndex].first;
    y = freeSpaces[randomIndex].second;
    return true;
}

void Map::addOrganism(Organism* organism) {
    if (organism != nullptr) {
        organisms.push_back(organism);
    }
}

Organism* Map::getOrganismAt(int x, int y) const {
    for (Organism* org : organisms) {
        if (org->getX() == x && org->getY() == y) {
            return org;
        }
    }
    return nullptr;
}

bool Map::isInside(int x, int y) const {
    return (x > 0 && x < width - 1 && y > 0 && y < height - 1);
}

void Map::removeOrganism(Organism* organism) {
    auto it = std::find(organisms.begin(), organisms.end(), organism);
    if (it != organisms.end()) {
        organisms.erase(it);
        toDelete.push_back(organism);
    }
}

void Map::playTurn() {
    std::vector<Organism*> sortedOrganisms = organisms;
    std::sort(sortedOrganisms.begin(), sortedOrganisms.end(), [](Organism* a, Organism* b) {
        if (a->getInitiative() == b->getInitiative()) {
            return a->getAge() > b->getAge();
        }
        return a->getInitiative() > b->getInitiative();
    });

    for (Organism* org : sortedOrganisms) {
        if (org == nullptr) continue;

        if (std::find(organisms.begin(), organisms.end(), org) == organisms.end()) {
            continue;
        }

        org->action();
    }

    for (Organism* org : toDelete) {
        delete org;
    }
    toDelete.clear();

}

bool Map::getRandomFreePosition(int& x, int& y) const {
    const int maxTries = width * height;

    for (int i = 0; i < maxTries; ++i) {
        int rx = 1 + rand() % (width - 2);
        int ry = 1 + rand() % (height - 2);

        if (isInside(rx, ry) && getOrganismAt(rx, ry) == nullptr) {
            x = rx;
            y = ry;
            return true;
        }
    }
    return false;
}

void Map::logEvent(const std::string& msg) {
    events.push_back(msg);
    if (events.size() > 20) {
        events.pop_front();
    }
}

const std::deque<std::string>& Map::getEvents() const {
    return events;
}

void Map::clearEvents() {
    events.clear();
}

std::string Map::getOrganismName(const Organism* org) const {
    if (dynamic_cast<const Wolf*>(org)) return "Wolf";
    if (dynamic_cast<const Sheep*>(org)) return "Sheep";
    if (dynamic_cast<const Fox*>(org)) return "Fox";
    if (dynamic_cast<const Turtle*>(org)) return "Turtle";
    if (dynamic_cast<const Antelope*>(org)) return "Antelope";
    if (dynamic_cast<const Cybersheep*>(org)) return "Cybersheep";
    if (dynamic_cast<const Grass*>(org)) return "Grass";
    if (dynamic_cast<const Milkweed*>(org)) return "Milkweed";
    if (dynamic_cast<const Gurana*>(org)) return "Guarana";
    if (dynamic_cast<const Nightshade*>(org)) return "Nightshade";
    if (dynamic_cast<const Hogweed*>(org)) return "Hogweed";
    if (dynamic_cast<const Human*>(org)) return "Human";
    return "Unknown";
}

Organism* Map::getAnimalAt(int x, int y) const {
    for (Organism* org : organisms) {
        if (org->getX() == x && org->getY() == y && dynamic_cast<Animal*>(org) != nullptr) {
            return org;
        }
    }
    return nullptr;
}

Organism* Map::getPlantAt(int x, int y) const {
    for (Organism* org : organisms) {
        if (org->getX() == x && org->getY() == y && dynamic_cast<Plant*>(org) != nullptr) {
            return org;
        }
    }
    return nullptr;
}

const std::vector<Organism*>& Map::getOrganisms() const {
    return organisms;
}

void Map::clearWorld() {
    std::unordered_set<Organism*> allPtrs;
    for (Organism* org : organisms) {
        if (org != nullptr) allPtrs.insert(org);
    }
    for (Organism* org : toDelete) {
        if (org != nullptr) allPtrs.insert(org);
    }

    for (Organism* org : allPtrs) {
        delete org;
    }

    organisms.clear();
    toDelete.clear();
    events.clear();
    generate();
}
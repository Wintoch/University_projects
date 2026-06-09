#include "game.hpp"
#include <cstdlib>
#include <ctime>
#include <functional>
#include <fstream>
#include <string>

Game::Game(int w, int h) : width(w), height(h), map(w, h) {
    initNcurses();
    win = newwin(height, width, 0, 0);

    const int infoHeight = 28;
    const int infoWidth = 40;
    infoWin = newwin(infoHeight, infoWidth, height, 0);
    scrollok(infoWin, FALSE);

    map.generate();

    human = new Human(width / 2, height / 2, &map);
    map.addOrganism(human);

    using Creator = std::function<Organism*(int, int, Map*)>;

    struct SpawnSpec{
        int count;
        Creator create;
    };

    std::vector<SpawnSpec> spawns = {
        {2, [](int x, int y, Map* map) { return new Sheep(x, y, map); }},
        {2, [](int x, int y, Map* map) { return new Wolf(x, y, map); }},
        {2, [](int x, int y, Map* map) { return new Fox(x, y, map); }},
        {2, [](int x, int y, Map* map) { return new Turtle(x, y, map); }},
        {2, [](int x, int y, Map* map) { return new Antelope(x, y, map); }},
        {1, [](int x, int y, Map* map) { return new Cybersheep(x, y, map); }},
        {3, [](int x, int y, Map* map) { return new Grass(x, y, map); }},
        {3, [](int x, int y, Map* map) { return new Milkweed(x, y, map); }},
        {3, [](int x, int y, Map* map) { return new Gurana(x, y, map); }},
        {3, [](int x, int y, Map* map) { return new Nightshade(x, y, map); }},
        {1, [](int x, int y, Map* map) { return new Hogweed(x, y, map); }}
    };

    for (const auto& spec : spawns) {
        for (int i = 0; i < spec.count; ++i) {
            int x, y;
            if (map.getRandomFreePosition(x, y)) {
                map.addOrganism(spec.create(x, y, &map));
            }
        }
    }
}

Game::~Game() {
    delwin(infoWin);
    delwin(win);
    endwin();
}

void Game::initNcurses() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);

    if (has_colors()) {
        start_color();
        use_default_colors();

        // 1 = wolf (gray)
        if (COLORS >= 256) {
            init_pair(1, 244, -1);
        } else {
            init_pair(1, COLOR_WHITE, -1);
        }

        // 2 = sheep (white)
        init_pair(2, COLOR_WHITE, -1);

        // 3 = fox (orange)
        if (COLORS >= 8 && can_change_color()) {
            init_color(10, 1000, 647, 0);
            init_pair(3, 10, -1);
        } else if (COLORS >= 256) {
            init_pair(3, 208, -1);
        } else {
            init_pair(3, COLOR_YELLOW, -1);
        }

        // 4 = turtle (green)
        if (COLORS >= 256) {
            init_pair(4, 28, -1);
        } else {
            init_pair(4, COLOR_GREEN, -1);
        }

        //5 = antelope (brown)
        if (COLORS >= 8 && can_change_color()) {
            init_color(11, 647, 412, 235);
            init_pair(5, 11, -1);
        } else if (COLORS >= 256) {
            init_pair(5, 94, -1);
        } else {
            init_pair(5, COLOR_YELLOW, -1);
        }

        //6 = cybersheep (cyan)
        if (COLORS >= 256) {
            init_pair(6, 51, -1);
        } else {
            init_pair(6, COLOR_CYAN, -1);
        }

        //7 = grass (green)
        if (COLORS >= 256) {
            init_pair(7, 46, -1);
        } else {
            init_pair(7, COLOR_GREEN, -1);
        }

        //8 = milkweed (yellow)
        if (COLORS >= 256) {
            init_pair(8, 226, -1);
        } else {
            init_pair(8, COLOR_YELLOW, -1);
        }

        //9 = guarana (red)
        if (COLORS >= 256) {
            init_pair(9, 160, -1);
        } else {
            init_pair(9, COLOR_RED, -1);
        }

        //10 = nightshade (violet/pink)
        if (COLORS >= 256) {
            init_pair(10, 200, -1);
        } else {
            init_pair(10, COLOR_MAGENTA, -1);
        }

        //11 hogweed (blue)
        if (COLORS >= 256) {
            init_pair(11, 27, -1);
        } else {
            init_pair(11, COLOR_BLUE, -1);
        }
    }

    refresh();

}

void Game::drawInfoPanel() {
    werase(infoWin);
    box(infoWin, 0, 0);

    const int panelH = getmaxy(infoWin);
    const int panelW = getmaxx(infoWin);

    struct LegendItem { short cp; const char* label; bool dim; };
    const LegendItem items[] = {
        {1,  "Wolf", true},
        {2,  "Sheep", false},
        {3,  "Fox", false},
        {4,  "Turtle", false},
        {5,  "Antelope", false},
        {6,  "Cybersheep", false},
        {7,  "Grass", false},
        {8,  "Milkweed", false},
        {9,  "Guarana", false},
        {10, "Nightshade", false},
        {11, "Hogweed", false},
    };
    const int itemCount = sizeof(items) / sizeof(items[0]);

    auto printClipped = [&](int row, int col, const std::string& text) {
        if (row <= 0 || row >= panelH - 1) return;
        if (col <= 0 || col >= panelW - 1) return;

        int maxLen = panelW - col - 1;
        if (maxLen <= 0) return;

        mvwaddnstr(infoWin, row, col, text.c_str(), maxLen);
    };

    int activeTurnsDisplay = human->isAbilityActive()
        ? (human->getAbilityTurnsLeft() + 1)
        : 0;

    // Status umiejetnosci (gora)
    mvwprintw(infoWin, 1, 2, "Umiejetnosc aktywna: %s",
              human->isAbilityActive() ? "TAK" : "NIE");
    mvwprintw(infoWin, 2, 2, "Mozna aktywowac: %s",
              human->canActivateAbility() ? "TAK" : "NIE");
    mvwprintw(infoWin, 3, 2, "Tury aktywne: %d, cooldown: %d",
              activeTurnsDisplay, human->getCooldownTurnsLeft());
    
    int currentPower = human->isAbilityActive()
        ? (activeTurnsDisplay + 5)
        : human->getStrength();

    mvwprintw(infoWin, 4, 2, "Obecna moc Human: %d", currentPower);

    printClipped(5, 2, "Sterowanie-strzalki, P-umiejetnosc");
    printClipped(6, 2, "K-zapis, L-wczytaj, Q-wyjscie");

    mvwprintw(infoWin, 8, 2, "Legenda:");
    const int legendStartRow = 9;
    const int leftCol = 2;
    const int rightCol = panelW / 2;
    const int rowsPerCol = (itemCount + 1) / 2;

    for (int i = 0; i < itemCount; ++i) {
        int colBase = (i < rowsPerCol) ? leftCol : rightCol;
        int row = legendStartRow + (i % rowsPerCol);
        if (row >= panelH - 2) break;

        const LegendItem& it = items[i];
        if (it.dim) wattron(infoWin, COLOR_PAIR(it.cp) | A_DIM);
        else wattron(infoWin, COLOR_PAIR(it.cp));

        mvwaddch(infoWin, row, colBase + 1, ACS_CKBOARD);

        if (it.dim) wattroff(infoWin, COLOR_PAIR(it.cp) | A_DIM);
        else wattroff(infoWin, COLOR_PAIR(it.cp));

        mvwprintw(infoWin, row, colBase + 5, "- %s", it.label);
    }

    // Logi pod legenda
    int logsTitleRow = legendStartRow + rowsPerCol + 1;
    if (logsTitleRow < panelH - 1) {
        mvwprintw(infoWin, logsTitleRow, 2, "Akcje (ta tura):");
    }

    const auto& events = map.getEvents();
    int logRow = logsTitleRow + 1;
    int lastUsableRow = panelH - 2;
    int maxRows = lastUsableRow - logRow + 1;
    if (maxRows < 0) maxRows = 0;

    int start = (events.size() > static_cast<size_t>(maxRows))
              ? static_cast<int>(events.size()) - maxRows
              : 0;

    for (int i = start; i < static_cast<int>(events.size()) && logRow <= lastUsableRow; ++i, ++logRow) {
        printClipped(logRow, 2, events[i]);
    }

    printClipped(panelH - 2, 2, "Jakub Wintoch - s208231");
    wrefresh(infoWin);
}

std::string Game::getTypeToken(const Organism* org) const {
    if (dynamic_cast<const Human*>(org)) return "HUMAN";
    if (dynamic_cast<const Wolf*>(org)) return "WOLF";
    if (dynamic_cast<const Sheep*>(org)) return "SHEEP";
    if (dynamic_cast<const Fox*>(org)) return "FOX";
    if (dynamic_cast<const Turtle*>(org)) return "TURTLE";
    if (dynamic_cast<const Antelope*>(org)) return "ANTELOPE";
    if (dynamic_cast<const Cybersheep*>(org)) return "CYBERSHEEP";
    if (dynamic_cast<const Grass*>(org)) return "GRASS";
    if (dynamic_cast<const Milkweed*>(org)) return "MILKWEED";
    if (dynamic_cast<const Gurana*>(org)) return "GUARANA";
    if (dynamic_cast<const Nightshade*>(org)) return "NIGHTSHADE";
    if (dynamic_cast<const Hogweed*>(org)) return "HOGWEED";
    return "UNKNOWN";
}

Organism* Game::createOrganismByType(const std::string& type, int x, int y) {
    if (type == "HUMAN") return new Human(x, y, &map);
    if (type == "WOLF") return new Wolf(x, y, &map);
    if (type == "SHEEP") return new Sheep(x, y, &map);
    if (type == "FOX") return new Fox(x, y, &map);
    if (type == "TURTLE") return new Turtle(x, y, &map);
    if (type == "ANTELOPE") return new Antelope(x, y, &map);
    if (type == "CYBERSHEEP") return new Cybersheep(x, y, &map);
    if (type == "GRASS") return new Grass(x, y, &map);
    if (type == "MILKWEED") return new Milkweed(x, y, &map);
    if (type == "GUARANA") return new Gurana(x, y, &map);
    if (type == "NIGHTSHADE") return new Nightshade(x, y, &map);
    if (type == "HOGWEED") return new Hogweed(x, y, &map);
    return nullptr;
}

bool Game::saveToFile(const std::string& path) const {
    std::ofstream out(path);
    if (!out.is_open()) return false;

    const auto& orgs = map.getOrganisms();

    out << "WORLD_SNAPSHOT_V1\n";
    out << "MAP " << map.getWidth() << " " << map.getHeight() << "\n";
    out << "COUNT " << orgs.size() << "\n";

    for (Organism* org : orgs) {
        const std::string type = getTypeToken(org);
        if (type == "UNKNOWN") continue;

        out << type << " "
            << org->getX() << " "
            << org->getY() << " "
            << org->getStrength() << " "
            << org->getInitiative() << " "
            << org->getAge();

        const Human* h = dynamic_cast<const Human*>(org);
        if (h != nullptr) {
            out << " "
                << (h->isAbilityActive() ? 1 : 0) << " "
                << h->getAbilityTurnsLeft() << " "
                << h->getCooldownTurnsLeft() << " "
                << (h->isAbilityRequested() ? 1 : 0) << " "
                << h->getPendingDx() << " "
                << h->getPendingDy();
        }

        out << "\n";
    }

    return true;
}

bool Game::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) return false;

    std::string magic;
    if (!(in >> magic) || magic != "WORLD_SNAPSHOT_V1") return false;

    std::string mapTag;
    int fileW = 0, fileH = 0;
    if (!(in >> mapTag >> fileW >> fileH) || mapTag != "MAP") return false;

    if (fileW != map.getWidth() || fileH != map.getHeight()) {
        map.logEvent("Rozmiar mapy w pliku nie zgadza sie z aktualna gra.");
        return false;
    }

    std::string countTag;
    int count = 0;
    if (!(in >> countTag >> count) || countTag != "COUNT" || count < 0) return false;

    map.clearWorld();
    human = nullptr;

    for (int i = 0; i < count; ++i) {
        std::string type;
        int x = 0, y = 0, str = 0, init = 0, age = 0;

        if (!(in >> type >> x >> y >> str >> init >> age)) {
            map.clearWorld();
            return false;
        }

        Organism* org = createOrganismByType(type, x, y);
        if (org == nullptr) {
            map.clearWorld();
            return false;
        }

        org->setPosition(x, y);
        org->setStrength(str);
        org->setInitiative(init);
        org->setAge(age);

        if (type == "HUMAN") {
            int active = 0, activeLeft = 0, cooldownLeft = 0, requested = 0, pdx = 0, pdy = 0;
            if (!(in >> active >> activeLeft >> cooldownLeft >> requested >> pdx >> pdy)) {
                delete org;
                map.clearWorld();
                return false;
            }

            Human* h = dynamic_cast<Human*>(org);
            h->setAbilityState(active != 0, activeLeft, cooldownLeft, requested != 0);
            h->setPendingDirection(pdx, pdy);
            human = h;
        }

        map.addOrganism(org);
    }

    if (human == nullptr) {
        human = new Human(map.getWidth() / 2, map.getHeight() / 2, &map);
        //map.addOrganism(human);
    }

    map.clearEvents();
    map.logEvent("Wczytano stan swiata z pliku.");
    return true;
}

void Game::run() {
    int ch;
    while ((ch = getch()) != 'q') {
        bool advanceTurn = true;
        map.clearEvents();

        if (ch == KEY_UP) human->setDirection(0, -1);
        else if (ch == KEY_DOWN) human->setDirection(0, 1);
        else if (ch == KEY_LEFT) human->setDirection(-1, 0);
        else if (ch == KEY_RIGHT) human->setDirection(1, 0);
        else if (ch == 'p' || ch == 'P') human->requestAbilityActivation();
        else if (ch == 'k' || ch == 'K') {
            if (saveToFile("config.txt")) map.logEvent("Zapisano stan swiata do config.txt");
            else map.logEvent("Blad zapisu pliku config.txt");
            advanceTurn = false;
        } else if (ch == 'l' || ch == 'L') {
            if (!loadFromFile("config.txt")) map.logEvent("Blad wczytywania pliku config.txt");
            advanceTurn = false;
        }

        if (advanceTurn) {
            map.playTurn();
        }

        map.draw(win);
        drawInfoPanel();
    }
}
package Java;

import java.util.ArrayList;

import Java.animals.Wolf;
import Java.animals.Sheep;
import Java.animals.Fox;
import Java.animals.Turtle;
import Java.animals.Antelope;
import Java.plants.Grass;
import Java.plants.Guarana;
import Java.plants.Milkweed;
import Java.plants.NightShade;
import Java.animals.Human;


public class Game {
    private Board board;

    public Board getBoard() { return board; }

    public void switchBoard() {
        if (board == null || !board.isHexagonal()) {
            board = new HexBoard(rows, cols);
        } else {
            board = new SquareBoard(rows, cols);
        }
        clearWorld();
        initWorld();
    }

    private final int rows, cols;
    private final Organism[][] cells;

    public int getRows() { return rows; }
    public int getCols() { return cols; }

    public void clearWorld() {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                cells[r][c] = null;
            }
        }
        organisms.clear();
    }

    public void setHuman(Human human) {
        this.human = human;
    }

    public ArrayList<Organism> organisms = new ArrayList<Organism>();

    private final ArrayList<String> turnLogs = new ArrayList<>();

    public void beginTurnLogs() {
        turnLogs.clear();
    }

    public void logEvent(String msg) {
        turnLogs.add(msg);
    }

    public ArrayList<String> getTurnLogs() {
        return new ArrayList<>(turnLogs);
    }

    private Human human;

    public Game(int rows, int cols) {
        this.rows = rows;
        this.cols = cols;
        this.cells = new Organism[rows][cols];
        this.board = new SquareBoard(rows, cols);
    }

    public boolean isInside(int row, int col) {
        return board.isInside(row, col);
    }

    public void setCell(int r, int c, Organism organism) { cells[r][c] = organism; }

    public Organism getOrganismAt(int r, int c) {
        if (isInside(r, c)) return cells[r][c];
        return null;
    }

    public void addOrganismAt(int r, int c, Organism org) {
        if (isInside(r, c) && cells[r][c] == null) {
            cells[r][c] = org;
            organisms.add(org);
        }
    }

    public void activateHumanSpecial(){
        if (human == null) return;

        if(human.activateSpecial()) {
            logEvent("Human activated special ability!");
        } else if(human.isSpecialActive()){
            logEvent("Human's special ability is already active!");
        }else {
            logEvent("Human's special ability is on cooldown for " + human.getCooldownLeft() + " more turns.");
        }
    }

    public boolean isHumanSpecialActive() {
        return human != null && human.isSpecialActive();
    }

    public int getHumanStrength() {
        return human != null ? human.getStrength() : 0;
    }

    public int getHumanCooldown() {
        return human != null ? human.getCooldownLeft() : 0;
    }

    public boolean canUseAbility() {
        return human != null && human.canUseSpecial();
    }

    int[] getRandomXY() {
        int Max_ATTEMPTS = rows * cols;
        for (int attempt = 0; attempt < Max_ATTEMPTS; attempt++) {
            int x = (int) (Math.random() * rows);
            int y = (int) (Math.random() * cols);
            if (getOrganismAt(x, y) == null) {
                return new int[]{x, y};
            }
        }
        return null;
    }

    private void spawnHumanAtCenter() {
        int centerX = rows / 2;
        int centerY = cols / 2;

        if (cells[centerX][centerY] != null) {
            int[] pos = getRandomXY();
            if (pos == null) return;
            centerX = pos[0];
            centerY = pos[1];
        }

        human = new Human(centerX, centerY, this);
        cells[centerX][centerY] = human;
        organisms.add(human);
    }

    public void populateWorld(String name, int count) {
        for (int i = 0; i < count; i++) {
            int[] pos = getRandomXY();
            if (pos == null) return;


            Organism newOrg = null;
            switch (name) {
                case "Wolf":
                    newOrg = new Wolf(9, 5, pos[0], pos[1], this);
                    break;
                case "Sheep":
                    newOrg = new Sheep(4, 4, pos[0], pos[1], this);
                    break;
                case "Fox":
                    newOrg = new Fox(3, 7, pos[0], pos[1], this);
                    break;
                case "Turtle":
                    newOrg = new Turtle(2, 1, pos[0], pos[1], this);
                    break;
                case "Antelope":
                    newOrg = new Antelope(4, 4, pos[0], pos[1], this);
                    break;
                case "Grass":
                    newOrg = new Grass(pos[0], pos[1], this);
                    break;
                case "Guarana":
                    newOrg = new Guarana(pos[0], pos[1], this);
                    break;
                case "Milkweed":
                    newOrg = new Milkweed(pos[0], pos[1], this);
                    break;
                case "NightShade":
                    newOrg = new NightShade(pos[0], pos[1], this);
                    break;
            }

            if(newOrg != null) {
                addOrganismAt(pos[0], pos[1], newOrg);
            }
        }
    }

    public void initWorld(){
        spawnHumanAtCenter();
        populateWorld("Wolf", 2);
        populateWorld("Sheep", 2);
        populateWorld("Fox", 2);
        populateWorld("Turtle", 2);
        populateWorld("Antelope", 2);
        populateWorld("Grass", 1);
        populateWorld("Guarana", 1);
        populateWorld("Milkweed", 1);
        populateWorld("NightShade", 1);
    }

    public void setHumanDirection(int dir) {
        if (human != null) {
            human.setDirection(dir);
        }
    }

    public void playTurn() {
        ArrayList<Organism> turnOrder = new ArrayList<>(organisms);

        turnOrder.sort((a, b) -> {
            if (a.initiative != b.initiative) return b.initiative - a.initiative;
            return b.age - a.age;
        });

        for (Organism org : turnOrder) {
            if (!organisms.contains(org)) continue;
            org.action();
        }
    }

    public Organism createOrganismByName(String name, int r, int c) {
        switch (name) {
            case "Wolf": return new Wolf(9, 5, r, c, this);
            case "Sheep": return new Sheep(4, 4, r, c, this);
            case "Fox": return new Fox(3, 7, r, c, this);
            case "Turtle": return new Turtle(2, 1, r, c, this);
            case "Antelope": return new Antelope(4, 4, r, c, this);
            case "Grass": return new Grass(r, c, this);
            case "Guarana": return new Guarana(r, c, this);
            case "Milkweed": return new Milkweed(r, c, this);
            case "NightShade": return new NightShade(r, c, this);
            default: return null;
        }
    }
}
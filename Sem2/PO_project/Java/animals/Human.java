package Java.animals;
import Java.Animal;
import Java.Board;
import Java.Game;
import Java.Organism;
import Java.Plant;

import java.awt.Color;

public class Human extends Animal {
    private int direction = -1;
    private static final int BASE_STRENGTH = 5;
    private static final int SPECIAL_STRENGTH = 10;
    private boolean specialActive = false;
    private int turnsSinceActivation = -1;
    private static final int SPECIAL_COOLDOWN_TURNS = 5;
    private int cooldownleft = 0;

    public int getCooldownLeft() {
        return cooldownleft;
    }

    public int getTurnsSinceActivation() {
        return turnsSinceActivation;
    }

    public void restoreSpecialState(int strength, int age, int active, int cooldown, int turns) {
        this.strength = strength;
        this.age = age;
        this.specialActive = (active == 1);
        this.cooldownleft = cooldown;
        this.turnsSinceActivation = turns;
    }

    public boolean canUseSpecial() {
        return !specialActive && cooldownleft == 0;
    }

    public boolean activateSpecial() {
        if (specialActive || cooldownleft > 0) return false;
        specialActive = true;
        turnsSinceActivation = 0;
        this.strength = SPECIAL_STRENGTH;
        return true;
    }

    public boolean isSpecialActive() {
        return specialActive;
    }

    private void updateSpecialStatus() {
    if (specialActive) {
        if (turnsSinceActivation > 0) {
            this.strength -= 1;
        }

        turnsSinceActivation++;

        if (this.strength <= BASE_STRENGTH) {
            this.strength = BASE_STRENGTH;
            specialActive = false;
            turnsSinceActivation = -1;
            cooldownleft = SPECIAL_COOLDOWN_TURNS;
        }
    } else if (cooldownleft > 0) {
        cooldownleft--;
    }
    }

    public Human(int x, int y, Game game) {
        super(5, 4, x, y, game);
        this.predator = true;
    }

    @Override
    public void action() {
        updateSpecialStatus();
        move();
        age++;
    }

    @Override
    public Color getColor() {
        return new Color(0, 0, 255); // blue
    }

    @Override
    protected Animal reproduce(int x, int y) {return null;}

    @Override
    public Organism getOrganism() {
        return this;
    }

    public void setDirection(int direction) { this.direction = direction; }
    
    @Override
    protected void move(){
        if (direction < 0) return;

        Board board = game.getBoard();
        int[] next = board.step(x, y, direction, 1);
        int nx = next[0];
        int ny = next[1];

        if (!game.isInside(nx, ny)) {
            direction = -1;
            return; 
        }

        Organism other = game.getOrganismAt(nx, ny);
        
        if (other == null) {
            leaveCell();
            x = nx;
            y = ny;
            game.setCell(x, y, this);
        } else if (other instanceof Plant) {
            Plant plant = (Plant) other;
            if (isPredator() && plant.ignoredByPredator()) {
                stepOnPlant(plant, nx, ny); // nie zjada, tylko stoi na trawie/mleczu
            } else {
                collision(other); // normalne zjedzenie
            }
        } else {
            collision(other);
        }
        
        direction = -1; 
    }

    
}

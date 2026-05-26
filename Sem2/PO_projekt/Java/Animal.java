package Java;

import java.awt.Color;

abstract public class Animal extends Organism {

    protected int range = 1;
    protected boolean sense = false;
    protected boolean predator = false;

    abstract public Color getColor();
    abstract public Organism getOrganism();
    abstract protected Animal reproduce(int x, int y);

    private Plant underPlant = null;

    protected void leaveCell() {
        if (game.getOrganismAt(x, y) != this) {
            underPlant = null;
            return;
        }
        if (underPlant != null) {
            game.setCell(x, y, underPlant);
            underPlant = null;
        } else {
            game.setCell(x, y, null);
        }
    }

    protected void stepOnPlant(Plant plant, int nx, int ny) {
        leaveCell();
        underPlant = plant;
        x = nx;
        y = ny;
        game.setCell(x, y, this);
    }

    public boolean isPredator() {
        return predator;
    }

    protected Animal(int strength, int initiative, int x, int y, Game game) {
        super(strength, initiative, x, y, game);
    }

    protected void action() {
        move();
        age++;
    }


    protected void move(){
        if (this instanceof Java.animals.Turtle) {
            if ((int) (Math.random() * 100) < 75) return;
        }

        final int MAX_ATTEMPTS = 3;

        for (int attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
            Board board = game.getBoard();
            int dirCount = board.getDirectionCount(x);

            int dir = (int) (Math.random() * dirCount);
            int[] next = board.step(x, y, dir, range);
            int nx = next[0];
            int ny = next[1];

            if (!game.isInside(nx, ny)) continue;

            Organism other = game.getOrganismAt(nx, ny);
            if (other == null) {
                leaveCell();
                x = nx; y = ny;
                game.setCell(x, y, getOrganism());
                return;
            } else {
                if (other != null) {
                    if(sense && other.strength > this.strength) {
                        continue;
                    }

                    if(other.canDeflect(this)) {
                        continue;
                    }

                    if (other instanceof Plant) {
                        Plant plant = (Plant) other;

                        if (isPredator() && plant.ignoredByPredator()) {
                            stepOnPlant(plant, nx, ny);
                            return;
                        }
                    }

                    collision(other);
                }
                return;
            }
        }
    }

    protected void collision(Organism other){
        if (this == other) return;
        if(this.getClass() == other.getClass()) {
            int[] empty = findEmptyNeighbor();
            if (empty != null) {
                game.logEvent(this.getClass().getSimpleName() + " rozmnozyl sie");
                Animal offspring = reproduce(empty[0], empty[1]);
                if(offspring != null){
                    game.addOrganismAt(empty[0], empty[1], offspring);
                }
            }
            return;
        };

        String killEat = "";
        if (this.strength >= other.strength) {
            if(other instanceof Plant) killEat = " zjadl ";
            else killEat = " zabil ";
            game.logEvent(this.getClass().getSimpleName() + killEat + other.getClass().getSimpleName());
            int oldX = this.x;
            int oldY = this.y;
            int targetX = other.x;
            int targetY = other.y;

            game.organisms.remove(other);

            game.setCell(oldX, oldY, null);
            this.x = targetX;
            this.y = targetY;
            game.setCell(targetX, targetY, this);
        } else {
            game.logEvent(other.getClass().getSimpleName() + " zabil " + this.getClass().getSimpleName());
            game.setCell(this.x, this.y, null);
            game.organisms.remove(this);
        }
    }
}

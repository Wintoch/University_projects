package Java;

import java.awt.Color;

abstract public class Organism {
    protected int strength;
    protected int initiative;
    protected int age;
    protected int x;
    protected int y;
    protected Game game;

    protected Organism(int strength, int initiative, int x, int y, Game game) {
        this.strength = strength;
        this.initiative = initiative;
        this.age = 0;
        this.x = x;
        this.y = y;
        this.game = game;
    }

    public abstract Color getColor();
    public abstract Organism getOrganism();
    abstract protected void collision(Organism other);
    abstract protected void action();

    public boolean canDeflect(Organism attacker) {
        return false;
    }

    public int getStrength() { return strength; }
    public void setStrength(int strength) { this.strength = strength; }
    public int getX() { return x; }
    public int getY() { return y; }

    protected int[] findEmptyNeighbor() {
        for (int[] p : game.getBoard().getNeighbors(x, y)) {
            int nx = p[0];
            int ny = p[1];
            if (game.getOrganismAt(nx, ny) == null) {
                return new int[]{nx, ny};
            }
        }
        return null;
    }
    
}

package Java.plants;

import Java.Game;
import Java.Organism;
import Java.Plant;
import java.awt.Color;

public class Grass extends Plant {
    public Grass(int x, int y, Game game) {
        super(0,0, x, y, game);
    }

    @Override
    public Color getColor() {
        return new Color(0, 255, 0);
    }

    @Override
    public boolean ignoredByPredator() {
        return true;
    }

    @Override
    public Organism getOrganism() {
        return this;
    }

    @Override
    protected Plant spread(int targetX, int targetY) {
        return new Grass(targetX, targetY, game);
    }
}
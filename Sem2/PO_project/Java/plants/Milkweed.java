package Java.plants;

import Java.Game;
import Java.Organism;
import Java.Plant;
import java.awt.Color;

public class Milkweed extends Plant {
    public Milkweed(int x, int y, Game game) {
        super(0,0, x, y, game);
    }

    @Override
    public Color getColor() {
        return new Color(255, 255, 0); //yellow
    }

    @Override
    public Organism getOrganism() {
        return this;
    }

    @Override
    public boolean ignoredByPredator() {
        return true;
    }

    @Override
        protected void action() {
        for (int i = 0; i < 3; i++) {
            tryToSpread();
        }
        age++;
    }

    @Override
    protected Plant spread(int targetX, int targetY) {
        return new Milkweed(targetX, targetY, game);
    }
    
}

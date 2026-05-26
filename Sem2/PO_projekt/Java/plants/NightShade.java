package Java.plants;

import Java.Game;
import Java.Organism;
import Java.Plant;
import java.awt.Color;

public class NightShade extends Plant {

    public NightShade(int x, int y, Game game) {
        super(99, 0, x, y, game);
    }

    @Override
    public Color getColor() {
        return new Color(128, 0, 128); // purple
    }

    @Override
    public Organism getOrganism() {
        return this;
    }

    @Override
    protected Plant spread(int targetX, int targetY) {
        return new NightShade(targetX, targetY, game);
    }

    @Override
    protected void collision(Organism attacker) {
        game.setCell(attacker.getX(), attacker.getY(), null);
        game.organisms.remove(attacker);
        
        super.collision(attacker);
    }
}
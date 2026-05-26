package Java.plants;

import Java.Game;
import Java.Organism;
import Java.Plant;
import java.awt.Color;

public class Guarana extends Plant {

    public Guarana(int x, int y, Game game) {
        super(0, 0, x, y, game);
    }

    @Override
    public Color getColor() {
        return new Color(255, 105, 180); // karmazynowy lekki/jasny czerwony
    }

    @Override
    public Organism getOrganism() {
        return this;
    }

    @Override
    protected Plant spread(int targetX, int targetY) {
        return new Guarana(targetX, targetY, game);
    }

    @Override
    protected void collision(Organism attacker) {
        attacker.setStrength(attacker.getStrength() + 3);
        super.collision(attacker);
    }
}
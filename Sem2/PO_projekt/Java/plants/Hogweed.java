package Java.plants;

import Java.Animal;
import Java.Game;
import Java.Organism;
import Java.Plant;
import java.awt.Color;

public class Hogweed extends Plant {

    public Hogweed(int x, int y, Game game) {
        super(10, 0, x, y, game);
    }

    @Override
    public Color getColor() {
        return new Color(0, 255, 255); // cyan 
    }

    @Override
    public Organism getOrganism() {
        return this;
    }

    @Override
    protected Plant spread(int targetX, int targetY) {
        return new Hogweed(targetX, targetY, game);
    }

    @Override
    protected void action() {
        super.action();

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                
                int nx = x + dx;
                int ny = y + dy;

                if (game.isInside(nx, ny)) {
                    Organism neighbor = game.getOrganismAt(nx, ny);

                    if (neighbor instanceof Animal) {
                        game.setCell(nx, ny, null);
                        game.organisms.remove(neighbor);
                    }
                }
            }
        }
    }

    @Override
    protected void collision(Organism attacker) {
        game.organisms.remove(attacker);
        game.setCell(attacker.getX(), attacker.getY(), null); 
        super.collision(attacker); 
    }
}
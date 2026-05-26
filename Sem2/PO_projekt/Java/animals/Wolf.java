package Java.animals;
import Java.Animal;
import Java.Game;
import Java.Organism;
import java.awt.Color;

public class Wolf extends Animal {
    public Wolf(int strength, int initiative, int x, int y, Game game) {
        super(strength, initiative, x, y, game);
        this.predator = true;
    }

    @Override
    public Color getColor() {
        return new Color(128, 128, 128); // szary
    }

    @Override
    public Organism getOrganism() {
        return this;
    }

    @Override
    protected Animal reproduce(int x, int y) {
        return new Wolf(9, 5, x, y, game);
    }
    
}

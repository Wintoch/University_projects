package Java.animals;
import Java.Animal;
import Java.Game;
import Java.Organism;
import java.awt.Color;

public class Fox extends Animal {
    public Fox(int strength, int initiative, int x, int y, Game game) {
        super(strength, initiative, x, y, game);
        this.sense = true;
    }

    @Override
    public Color getColor() {
        return new Color(255, 165, 0); // orange
    }

    @Override
    public Organism getOrganism() {
        return this;
    }

    @Override
    protected Animal reproduce(int x, int y) {
        return new Fox(3, 7, x, y, game);
    }
}

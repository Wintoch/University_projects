package Java.animals;
import Java.Animal;
import Java.Organism;
import Java.Game;
import java.awt.Color;

public class Sheep extends Animal {
    public Sheep(int strength, int initiative, int x, int y, Game game) {
        super(strength, initiative, x, y, game);
    }

    @Override
    public Color getColor() {
        return new Color(255, 255, 255); // white
    }

    @Override
    public Organism getOrganism() {
        return this;
    }

    @Override
    protected Animal reproduce(int x, int y) {
        return new Sheep(4, 4, x, y, game);
    }

}

package Java.animals;
import Java.Animal;
import Java.Game;
import Java.Organism;
import java.awt.Color;

public class Turtle extends Animal {
    public Turtle(int strength, int initiative, int x, int y, Game game) {
        super(strength, initiative, x, y, game);
    }

    @Override
    public Color getColor() {
        return new Color(0, 128, 0); // green
    }

    @Override
    public boolean canDeflect(Organism attacker) {
        if (attacker.getStrength() < 5) {
            return true;
        }
        return false;
    }

    @Override
    public Organism getOrganism() {
        return this;
    }

    @Override
    protected Animal reproduce(int x, int y) {
        return new Turtle(2, 1, x, y, game);
    }
    
}

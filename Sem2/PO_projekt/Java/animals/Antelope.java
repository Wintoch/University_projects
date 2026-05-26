package Java.animals;
import Java.Animal;
import Java.Game;
import Java.Organism;
import java.awt.Color;

public class Antelope extends Animal {
    public Antelope(int strength, int initiative, int x, int y, Game game) {
        super(strength, initiative, x, y, game);
        this.range = 2;
    }

    @Override
    public Color getColor() {
        return new Color(160, 82, 45); // brown
    }

    @Override
    public Organism getOrganism() {
        return this;
    }

    @Override
    protected Animal reproduce(int x, int y) {
        return new Antelope(4, 4, x, y, game);
    }
    
}

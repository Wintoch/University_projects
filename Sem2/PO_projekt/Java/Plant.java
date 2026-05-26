package Java;

abstract public class Plant extends Organism {
    protected final double SPREAD_PROBABILITY = 0.05;

    protected Plant(int strength, int initiative, int x, int y, Game game) {
        super(strength, 0, x, y, game);
    }

    abstract protected Plant spread(int spreadX, int spreadY);

    public boolean ignoredByPredator() {
        return false;
    }

    protected void tryToSpread() {
        if(Math.random() < SPREAD_PROBABILITY) {
            int[] empty = findEmptyNeighbor();
            if (empty != null) {
                game.logEvent(this.getClass().getSimpleName() + " rozprzestrzenil sie");
                Plant offspring = spread(empty[0], empty[1]);
                game.addOrganismAt(empty[0], empty[1], offspring);
            }
        }
        age++;
    }

    @Override
    protected void action() {
        tryToSpread();
        age++;
    }

    @Override
    protected void collision(Organism other) {
        game.logEvent(other.getClass().getSimpleName() + " zjadl " + this.getClass().getSimpleName());
        game.setCell(this.x, this.y, null);
        game.organisms.remove(this);
    }
    
}

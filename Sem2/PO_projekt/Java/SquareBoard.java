package Java;

public class SquareBoard extends Board {
    private static final int[][] DELTAS = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };

    public SquareBoard(int rows, int cols) {
        super(rows, cols);
    }

    @Override
    public int[][] getNeighborDeltas(int row) {
        return DELTAS;
    }

    @Override
    public boolean isHexagonal() { return false; }
}

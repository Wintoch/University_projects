package Java;

public class HexBoard extends Board {
    private static final int[][] EVEN = {
            {-1, -1}, { -1, 0},
        {0,  -1},         { 0,  1},
            {1,  -1}, { 1,  0}
    };

    private static final int[][] ODD = {
            { -1, 0}, { -1, 1},
        {0,  -1},          { 0,  1},
            { 1,  0}, { 1,  1}
    };

    public HexBoard(int rows, int cols) {
        super(rows, cols);
    }

    @Override
    public int[][] getNeighborDeltas(int row) {
        return (row % 2 == 0) ? EVEN : ODD;
    }

    @Override
    public boolean isHexagonal() { return true; }
}

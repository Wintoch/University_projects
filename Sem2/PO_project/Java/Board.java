package Java;

import java.util.ArrayList;

public abstract class Board {
    protected final int rows;
    protected final int cols;

    protected Board(int rows, int cols) {
        this.rows = rows;
        this.cols = cols;
    }

    public int getRows() { return rows; }
    public int getCols() { return cols; }

    public boolean isInside(int row, int col) {
        return row >= 0 && row < rows && col >= 0 && col < cols;
    }

    public abstract int[][] getNeighborDeltas(int row);

    public int getDirectionCount(int row) {
        return getNeighborDeltas(row).length;
    }

    public int[] step(int r, int c, int dir, int steps){
        int cr = r;
        int cc = c;
        for(int i = 0; i < steps; i++) {
            int[][] deltas = getNeighborDeltas(cr);
            int dr = deltas[dir][0];
            int dc = deltas[dir][1];
            cr += dr;
            cc += dc;
        }
        return new int[]{cr, cc};
    }

    public ArrayList<int[]> getNeighbors(int r, int c) {
        ArrayList<int[]> out = new ArrayList<>();
        int[][] deltas = getNeighborDeltas(r);
        for (int[] d : deltas){
            int nr = r + d[0];
            int nc = c + d[1];
            if (isInside(nr, nc)) {
                out.add(new int[]{nr, nc});
            }
        }
        return out;
    }

    public abstract boolean isHexagonal();
}

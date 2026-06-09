package Java;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.HashMap;
import java.util.Map;
import java.util.function.BiConsumer;

public class HexBoardPanel extends JPanel {
    private final Game game;
    private final int size;
    private final Map<Point, Polygon> hexes = new HashMap<>();
    private BiConsumer<Integer, Integer> onCellClick;

    public HexBoardPanel(Game game, int size) {
        this.game = game;
        this.size = size;
        buildHexes();
        setBackground(Color.black);

        addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                Point cell = findCell(e.getPoint());
                if (cell != null && onCellClick != null) {
                    onCellClick.accept(cell.x, cell.y);
                }
            }
        });
    }

    public void setOnCellClick(BiConsumer<Integer, Integer> handler) {
        this.onCellClick = handler;
    }

    private Polygon buildHex(double cx, double cy, double r) {
        int[] xs = new int[6];
        int[] ys = new int[6];
        for (int i = 0; i < 6; i++) {
            double angle = Math.toRadians(60 * i - 30);
            xs[i] = (int)Math.round(cx + r * Math.cos(angle));
            ys[i] = (int)Math.round(cy + r * Math.sin(angle));
        }
        return new Polygon(xs, ys, 6);
    }

    private void buildHexes() {
        hexes.clear();
        int rows = game.getBoard().getRows();
        int cols = game.getBoard().getCols();

        double w = Math.sqrt(3) * size;
        double h = 2 * size;
        double vStep = 1.5 * size;

        double padX = size;
        double padY = size;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                double cx = padX + w * (c + 0.5 * (r & 1));
                double cy = padY + vStep * r;
                Polygon p = buildHex(cx, cy, size);
                hexes.put(new Point(r, c), p);
            }
        }

        int width = (int)(padX * 2 + w * (cols + 0.5));
        int height = (int)(padY * 2 + vStep * (rows - 1) + h);
        setPreferredSize(new Dimension(width, height));
    }

    private Point findCell(Point p) {
        for (Map.Entry<Point, Polygon> e : hexes.entrySet()) {
            if (e.getValue().contains(p)) {
                return e.getKey();
            }
        }
        return null;
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        for (Map.Entry<Point, Polygon> e : hexes.entrySet()) {
            Point cell = e.getKey();
            Polygon poly = e.getValue();
            Organism org = game.getOrganismAt(cell.x, cell.y);

            if (org != null) {
                g.setColor(org.getColor());
                g.fillPolygon(poly);
            } else {
                g.setColor(Color.black);
                g.fillPolygon(poly);
            }

            g.setColor(new Color(40, 40, 40));
            g.drawPolygon(poly);
        }
    }

}

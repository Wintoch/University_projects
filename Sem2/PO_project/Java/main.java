package Java;

import javax.swing.SwingUtilities;

public class main {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            MapWindow window = new MapWindow(10,10);
            window.setVisible(true);
        });
    }
}

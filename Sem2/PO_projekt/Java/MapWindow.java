package Java;

import javax.swing.*;
import javax.swing.border.TitledBorder;

import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.ArrayList;

public class MapWindow extends JFrame implements KeyListener {
    public final int ROWS;
    public final int COLS;

    private final Game game;
    private final JButton[][] buttons;

    private final JPanel gameBoard;
    private final JPanel menu;
    private JTextArea logArea;

    private JComponent boardView;
    private HexBoardPanel hexPanel;

    public MapWindow(int rows, int cols) {
        this.ROWS = rows;
        this.COLS = cols;

        this.game = new Game(ROWS, COLS);
        this.buttons = new JButton[ROWS][COLS];

        this.gameBoard = new JPanel(new BorderLayout());
        this.menu = new JPanel(new BorderLayout());

        configureWindow();
        configurePanels();
        buildLayout();
        buildBoardView();
        game.initWorld();
        fillMenu();
        game.beginTurnLogs();
        showLogs(game.getTurnLogs());
        refreshBoard();
        
        this.addKeyListener(this);
        this.setFocusable(true);
    }

    @Override
    public void keyPressed(KeyEvent e) {
        int keyCode = e.getKeyCode();
        boolean processed = false;

        game.beginTurnLogs();

        int dir = -1;
        boolean isHex = game.getBoard().isHexagonal();

        if(!isHex){
            if (keyCode == KeyEvent.VK_UP) dir = 1;
            if (keyCode == KeyEvent.VK_DOWN) dir = 6;
            if (keyCode == KeyEvent.VK_LEFT) dir = 3;
            if (keyCode == KeyEvent.VK_RIGHT) dir = 4;
        } else {
        if (keyCode == KeyEvent.VK_W) dir = 0; // NW
        if (keyCode == KeyEvent.VK_E) dir = 1; // NE
        if (keyCode == KeyEvent.VK_A) dir = 2; // W
        if (keyCode == KeyEvent.VK_D) dir = 3; // E
        if (keyCode == KeyEvent.VK_Z) dir = 4; // SW
        if (keyCode == KeyEvent.VK_X) dir = 5; // SE
        }

        if (dir >= 0) {
            game.setHumanDirection(dir);
            processed = true;
        }

        switch (keyCode) {
            case KeyEvent.VK_P:
                game.activateHumanSpecial();
                processed = true;
                break;
            case KeyEvent.VK_S:
                game.logEvent(Config.save(game));
                showLogs(game.getTurnLogs());
                return;
            case KeyEvent.VK_R:
                game.logEvent(Config.load(game));
                refreshBoard();
                showLogs(game.getTurnLogs());
                return;
            case KeyEvent.VK_B:
                game.switchBoard();
                buildBoardView();
                refreshBoard();
                showLogs(game.getTurnLogs());
                return;
        }

        if (processed) {
            game.playTurn();
            refreshBoard();
            showLogs(game.getTurnLogs());
        }
    }

    @Override public void keyTyped(KeyEvent e) {}
    @Override public void keyReleased(KeyEvent e) {}

    private void configureWindow() {
        setTitle(("Game Map - Jakub Wintoch 208231"));
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());
        setSize(1000,800);
        setLocationRelativeTo(null);      
    }

    private void configurePanels(){
        gameBoard.setBackground(Color.black);
        menu.setBackground(new Color(30,30,30));

        menu.setPreferredSize(new Dimension(1000, 180));

        TitledBorder boardBorder = BorderFactory.createTitledBorder("World");
        boardBorder.setTitleColor(Color.WHITE);
        gameBoard.setBorder(boardBorder);

        TitledBorder menuBorder = BorderFactory.createTitledBorder("Logs and Controls");
        menuBorder.setTitleColor(Color.WHITE);
        menu.setBorder(menuBorder);
    }

    private void buildLayout(){
        add(gameBoard, BorderLayout.CENTER);
        add(menu, BorderLayout.SOUTH);
    }

    private void onCellClick(int r, int c) {
            if (game.getOrganismAt(r, c) != null) {
        game.logEvent("This cell is occupied. Cannot add new organism.");
        showLogs(game.getTurnLogs());
        return;
    }

    String[] options = {
        "Wolf", "Sheep", "Fox", "Turtle", "Antelope",
        "Grass", "Guarana", "Milkweed", "NightShade"
    };

    String choice = (String) JOptionPane.showInputDialog(
        this,
        "Choose organism:",
        "Add organism",
        JOptionPane.PLAIN_MESSAGE,
        null,
        options,
        options[0]
    );

    if (choice == null) return;

    Organism org = game.createOrganismByName(choice, r, c);
        if (org != null) {
            game.addOrganismAt(r, c, org);
            game.logEvent("Dodano " + choice + " na [" + r + "," + c + "]");
            refreshBoard();
            showLogs(game.getTurnLogs());
        }
    }

    // private void fillBoard() {
    //     for (int r = 0; r < ROWS; r++) {
    //         for (int c = 0; c < COLS; c++) {
    //             JButton button = new JButton();
    //             button.setBackground(Color.black);
    //             button.setOpaque(true);
    //             button.setBorder(BorderFactory.createLineBorder(Color.BLACK));
    //             button.setFocusable(false);

    //             final int rr = r, cc = c;
    //             button.addActionListener(e -> onCellClick(rr, cc));

    //             buttons[r][c] = button;
    //             gameBoard.add(button);
    //         }
    //     }
    // }

    public void refreshBoard() {
        if (game.getBoard().isHexagonal()) {
            if (hexPanel != null) {
                hexPanel.repaint();
            }
        } else {
            for (int r = 0; r < ROWS; r++) {
                for (int c = 0; c < COLS; c++) {
                    Organism org = game.getOrganismAt(r, c);
                    if (org != null) {
                        buttons[r][c].setBackground(org.getColor());
                    } else {
                        buttons[r][c].setBackground(Color.black);
                    }
                }
            }
        }
        repaint();
    }

    private void buildBoardView() {
        if (boardView != null) {
            gameBoard.remove(boardView);
        }

        if (game.getBoard().isHexagonal()) {
            hexPanel = new HexBoardPanel(game, 16);
            hexPanel.setOnCellClick(this::onCellClick);
            boardView = new JScrollPane(hexPanel);
        } else {
            JPanel grid = new JPanel(new GridLayout(ROWS, COLS));
            for (int r = 0; r < ROWS; r++) {
                for (int c = 0; c < COLS; c++) {
                    JButton button = new JButton();
                    button.setBackground(Color.black);
                    button.setOpaque(true);
                    button.setBorder(BorderFactory.createLineBorder(Color.BLACK));
                    button.setFocusable(false);

                    final int rr = r, cc = c;
                    button.addActionListener(e -> onCellClick(rr, cc));

                    buttons[r][c] = button;
                    grid.add(button);
                }
            }
            boardView = grid;
        }

        gameBoard.add(boardView);
        gameBoard.revalidate();
        gameBoard.repaint();
    }

    private void fillMenu() {
        menu.setLayout(new BorderLayout());

        JPanel legendPanel = new JPanel(new GridLayout(0, 2, 10, 2));
        legendPanel.setOpaque(false);
        legendPanel.setPreferredSize(new Dimension(400, 160));

        Object[][] legendInfo = {
            {"Human", new Color(0, 0, 255)},
            {"Wolf", new Color(128, 128, 128)},
            {"Sheep", new Color(255, 255, 255)},
            {"Fox", new Color(255, 165, 0)},
            {"Turtle", new Color(0, 128, 0)},
            {"Antelope", new Color(160, 82, 45)},
            {"Grass", new Color(0, 255, 0)},
            {"Guarana", new Color(255, 105, 180)},
            {"Milkweed", new Color(255, 255, 0)},
            {"Nightshade", new Color(128, 0, 128)},
            {"Hogweed", new Color(0, 255, 255)}
        };

        for (Object[] item : legendInfo){
            String name = (String) item[0];
            Color color = (Color) item[1];

            JPanel itemPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 0));
            itemPanel.setOpaque(false);

            JPanel colorBox = new JPanel();
            colorBox.setBackground(color);
            colorBox.setPreferredSize(new Dimension(10, 10));
            colorBox.setBorder(BorderFactory.createLineBorder(Color.BLACK));

            JLabel label = new JLabel(name);
            label.setForeground(Color.WHITE);

            itemPanel.add(colorBox);
            itemPanel.add(label);
            legendPanel.add(itemPanel);
        }
        
        menu.add(legendPanel, BorderLayout.WEST);

        logArea = new JTextArea();
        logArea.setEditable(false);
        logArea.setBackground(new Color(20, 20, 20));
        logArea.setForeground(Color.WHITE);
        logArea.setLineWrap(true);
        logArea.setWrapStyleWord(true);
        logArea.setFocusable(false);
        
        JPanel logPanel = new JPanel(new BorderLayout());
        logPanel.add(logArea, BorderLayout.CENTER);

        menu.add(logPanel, BorderLayout.CENTER);
    }

    private void showLogs(ArrayList<String> logs) {
        clearLogs();

        String specialState = game.isHumanSpecialActive() ? "ON" : "OFF";
        String canUse = game.canUseAbility() ? "YES" : "NO";
        int cooldown = game.getHumanCooldown();

        addLog("Ability: " + specialState
                + " | Can use: " + canUse
                + " | Cooldown: " + cooldown
                + " | Strength Human: " + game.getHumanStrength());

        addLog("Save to file: S | Load from file: R");
        addLog("To switch board press: B");

        for (String line : logs) {
            addLog(line);
        }
    }

    public void addLog(String message) {
        logArea.append(message + "\n");
        logArea.setCaretPosition(logArea.getDocument().getLength());
    }

    public void clearLogs() {
        logArea.setText("");
    }
}

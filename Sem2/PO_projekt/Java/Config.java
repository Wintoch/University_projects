package Java;

import Java.animals.*;
import Java.plants.*;

import java.nio.file.*;
import java.util.ArrayList;
import java.util.List;

public class Config {
    private static final Path SAVE_PATH = Paths.get("Java", "config.txt");

    public static String save(Game game) {
        try {
            ArrayList<String> lines = new ArrayList<>();
            String boardType = game.getBoard().isHexagonal() ? "HEX" : "SQUARE";
            lines.add(boardType + " " + game.getRows() + " " + game.getCols());

            for (int r = 0; r < game.getRows(); r++) {
                for (int c = 0; c < game.getCols(); c++) {
                    Organism org = game.getOrganismAt(r, c);
                    if (org == null) continue;

                    String type = org.getClass().getSimpleName();
                    int strength = org.getStrength();
                    int age = org.age;

                    int extra1 = 0, extra2 = 0, extra3 = 0;
                    if (org instanceof Human) {
                        Human h = (Human) org;
                        extra1 = h.isSpecialActive() ? 1 : 0;
                        extra2 = h.getCooldownLeft();
                        extra3 = h.getTurnsSinceActivation();
                    }

                    lines.add(type + " " + r + " " + c + " " + strength + " " + age
                            + " " + extra1 + " " + extra2 + " " + extra3);
                }
            }

            Files.write(SAVE_PATH, lines);
            return "Zapisano stan do pliku";
        } catch (Exception e) {
            return "Blad zapisu: " + e.getMessage();
        }
    }

    public static String load(Game game) {
        try {
            if (!Files.exists(SAVE_PATH) || Files.size(SAVE_PATH) == 0) {
                return "No save file found to load";
            }

            List<String> lines = Files.readAllLines(SAVE_PATH);
            if (lines.isEmpty()) return "No save data found in file";

            String[] header = lines.get(0).trim().split(" ");
            if (header.length < 3) return "Invalid save header";

            String savedType = header[0];
            int savedRows = Integer.parseInt(header[1]);
            int savedCols = Integer.parseInt(header[2]);

            String currentType = game.getBoard().isHexagonal() ? "HEX" : "SQUARE";
            if (!savedType.equals(currentType)) {
                return "Cannot load: saved board type is " + savedType +
                    ", current board type is " + currentType;
            }

            if (savedRows != game.getRows() || savedCols != game.getCols()) {
                return "Cannot load: saved size (" + savedRows + "x" + savedCols +
                    ") differs from current (" + game.getRows() + "x" + game.getCols() + ")";
            }

            game.clearWorld();

            for (int i = 1; i < lines.size(); i++) {
                String line = lines.get(i).trim();
                if (line.isEmpty()) continue;

                String[] p = line.split(" ");
                String type = p[0];
                int r = Integer.parseInt(p[1]);
                int c = Integer.parseInt(p[2]);
                int strength = Integer.parseInt(p[3]);
                int age = Integer.parseInt(p[4]);
                int extra1 = Integer.parseInt(p[5]);
                int extra2 = Integer.parseInt(p[6]);
                int extra3 = Integer.parseInt(p[7]);

                Organism org = null;
                switch (type) {
                    case "Wolf": org = new Wolf(9, 5, r, c, game); break;
                    case "Sheep": org = new Sheep(4, 4, r, c, game); break;
                    case "Fox": org = new Fox(3, 7, r, c, game); break;
                    case "Turtle": org = new Turtle(2, 1, r, c, game); break;
                    case "Antelope": org = new Antelope(4, 4, r, c, game); break;
                    case "Grass": org = new Grass(r, c, game); break;
                    case "Guarana": org = new Guarana(r, c, game); break;
                    case "Milkweed": org = new Milkweed(r, c, game); break;
                    case "NightShade": org = new NightShade(r, c, game); break;
                    case "Human":
                        Human h = new Human(r, c, game);
                        h.restoreSpecialState(strength, age, extra1, extra2, extra3);
                        org = h;
                        game.setHuman(h);
                        break;
                }

                if (org != null) {
                    org.strength = strength;
                    org.age = age;
                    game.addOrganismAt(r, c, org);
                }
            }

            return "Loaded state from file";
        } catch (Exception e) {
            return "Load error: " + e.getMessage();
        }
    }
}
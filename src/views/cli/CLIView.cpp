#include "views/cli/CLIView.h"
#include <iostream>
#include <limits>
#include "termcolor.hpp"
#include <algorithm>
#include <map>
#include <sstream>

namespace hive::views::cli {
    void CLIView::displayMessage(const std::string &message) {
        std::cout << termcolor::reset << message << std::endl;
    }

    void CLIView::displayError(const std::string &error) {
        std::cout << termcolor::red << "[Erreur] " << error << termcolor::reset << std::endl;
    }

    std::string CLIView::askPlayerName(int playerNumber) {
        std::cout << termcolor::cyan << "Entrez le nom du joueur " << playerNumber << ": " << termcolor::reset;
        std::string name;
        std::getline(std::cin, name);
        if (name.empty()) {
            name = "Player " + std::to_string(playerNumber);
        }
        return name;
    }

    std::string CLIView::getUserCommand() {
        std::cout << termcolor::yellow
                << "Entrez une commande [place, move, undo, redo, quit]: "
                << termcolor::reset;
        std::string command;
        std::getline(std::cin, command);
        return command;
    }

    int CLIView::askInt(const std::string &prompt) const {
        std::cout << termcolor::magenta << prompt << ": " << termcolor::reset;
        int val;
        while (!(std::cin >> val)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << termcolor::red << "Entrée invalide, veuillez entrer un entier." << termcolor::reset <<
                    std::endl;
            std::cout << termcolor::magenta << prompt << ": " << termcolor::reset;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return val;
    }

    int CLIView::askOption(const std::string &prompt, const std::vector<std::string> &options) const {
        std::cout << termcolor::blue << prompt << termcolor::reset << std::endl;
        for (size_t i = 0; i < options.size(); ++i) {
            std::cout << "  " << i << ": " << options[i] << std::endl;
        }
        std::cout << "Votre choix: ";
        int choice;
        while (!(std::cin >> choice) || choice < 0 || (size_t) choice >= options.size()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << termcolor::red << "Choix invalide." << termcolor::reset << std::endl;
            std::cout << "Votre choix: ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return choice;
    }

    void CLIView::displayGameState(const hive::models::Game &game) const {
        // Afficher le titre de l'état du jeu
        printTitle("ÉTAT DE LA PARTIE");

        // Tour actuel
        std::cout << termcolor::green << "Tour n°" << game.getTurnNumber() << termcolor::reset << std::endl;

        // Joueur courant
        std::cout << "Joueur courant: " << termcolor::yellow << game.getCurrentPlayer().getName() << termcolor::reset <<
                std::endl;

        printSeparator();

        // Affichage du plateau
        displayBoard(game);

        printSeparator();

        // Informations supplémentaires (statut, etc.)
        if (auto [isGameOver, isDraw, winner] = game.getGameStatus(); isGameOver) {
            std::cout << termcolor::red << "La partie est terminée." << termcolor::reset;
            if (isDraw) {
                std::cout << " Match nul !" << std::endl;
            } else {
                std::cout << " Le gagnant est : " << termcolor::yellow << winner->getName() << termcolor::reset <<
                        std::endl;
            }
        } else {
            std::cout << "La partie est en cours." << std::endl;
        }

        std::cout << std::endl; // espace final
    }

    void CLIView::printTitle(const std::string &title) const {
        std::cout << termcolor::bold << termcolor::blue << "=== " << title << " ===" << termcolor::reset << std::endl;
    }

    void CLIView::printSeparator(const char sep, const int length) {
        for (int i = 0; i < length; ++i) std::cout << sep;
        std::cout << std::endl;
    }


    std::string getColoredString(const std::string &text, const std::string &colorName) {
        std::ostringstream oss;

        // Appliquer la couleur
        if (colorName == "red") {
            oss << termcolor::red;
        } else if (colorName == "blue") {
            oss << termcolor::blue;
        } else if (colorName == "green") {
            oss << termcolor::green;
        } else if (colorName == "yellow") {
            oss << termcolor::yellow;
        } else if (colorName == "magenta") {
            oss << termcolor::magenta;
        } else if (colorName == "cyan") {
            oss << termcolor::cyan;
        } else if (colorName == "white") {
            oss << termcolor::white;
        } else {
            // Couleur inconnue, par défaut blanc
            oss << termcolor::white;
        }

        // Ajouter le texte
        oss << text;

        // Réinitialiser la couleur
        oss << termcolor::reset;

        // Renvoyer la chaîne complète
        return oss.str();
    }


    static std::string pieceTypeToChar(hive::models::enums::PieceType type) {
        using PT = hive::models::enums::PieceType;
        switch (type) {
            case PT::QUEEN_BEE: return "Q";
            case PT::ANT: return "A";
            case PT::BEETLE: return "B";
            case PT::GRASSHOPPER: return "G";
            case PT::SPIDER: return "S";
            case PT::LADYBUG: return "L";
            case PT::MOSQUITO: return "M";
            case PT::PILLBUG: return "P";
            default: return "?";
        }
    }

    /**
     * @brief Construit l'ASCII d'un seul hexagone.
     *
     * Chaque hexagone fera ~6 lignes. On va s'inspirer de la forme donnée :
     *
     *    /•\
     *  /•   •\
     * |• HX  •|
     * |• PCE •|
     *  \•   •/
     *    \•/
     *
     * On ajustera pour afficher x,y,z ou le type de la pièce.
     *
     * Paramètres :
     * - color : couleur à appliquer au texte principal du hex.
     * - labelTop : texte affiché dans la première ligne interne (ex: "HX1").
     * - labelBot : texte affiché dans la seconde ligne interne (ex: "-B-" ou coords).
     *
     * Retourne un vector<string> de 6 lignes.
     */
    static std::vector<std::string> drawHexCell(const std::string &color, const std::string &labelTop,
                                                const std::string &labelBot) {
        // On s'assure que labelTop et labelBot tiennent dans 5 chars max.
        std::string top = labelTop.substr(0, 5);
        std::string bot = labelBot.substr(0, 5);

        // On va faire un hex de 7 chars de large
        // Forme (7 large):
        //    /•\      (3 espaces + /•\)
        //  /•   •\    (1 espace + /•   •\)
        // |•  top •|   (|•  hx1 •|)
        // |•  bot •|   (|•  -B- •|)
        //  \•   •/
        //    \•/
        // Les "•" sont décoratives, vous pouvez les remplacer par des espaces si vous préférez.
        // On utilise la couleur sur le contenu interne (top, bot).
        std::vector<std::string> hexLines;
        hexLines.push_back(std::string("   /•\\   "));
        hexLines.push_back(std::string(" /•   •\\ "));
        // top et bot doivent être centrés dans un espace de 5 chars.
        auto centerText = [](const std::string &txt, int width = 5) {
            int len = (int) txt.size();
            if (len >= width) return txt.substr(0, width);
            int pad = (width - len) / 2;
            return std::string(pad, ' ') + txt + std::string(width - pad - len, ' ');
        };

        const std::string cTop = centerText(top);
        const std::string cBot = centerText(bot);

        hexLines.push_back("|•" + getColoredString(cTop, color) + "•|");
        hexLines.push_back("|•" + getColoredString(cBot, color) + "•|");
        hexLines.push_back(" \\•   •/ ");
        hexLines.push_back("   \\•/   ");

        return hexLines;
    }

    void CLIView::displayBoard(const hive::models::Game &game) {
        const auto &boardMap = game.getBoard().getBoard();

        if (boardMap.empty()) {
            std::cout << termcolor::cyan << "Plateau:" << termcolor::reset << std::endl;
            std::cout << "Le plateau est vide." << std::endl;
            return;
        }

        std::cout << termcolor::cyan << "Plateau:" << termcolor::reset << std::endl;

        // Déterminer minX, maxX, minY, maxY se sont les max et la mins de int_8
        int minX = std::numeric_limits<int8_t>::max();
        int maxX = std::numeric_limits<int8_t>::min();
        int minY = std::numeric_limits<int8_t>::max();
        int maxY = std::numeric_limits<int8_t>::min();
        for (auto &kv: boardMap) {
            auto h = kv.first;
            int x = h.getX();
            int y = h.getY();
            minX = std::min(minX, x);
            maxX = std::max(maxX, x);
            minY = std::min(minY, y);
            maxY = std::max(maxY, y);
        }

        // Construire la grille
        std::map<std::pair<int, int>, std::vector<std::string> > hexGrid;

        for (auto &kv: boardMap) {
            auto h = kv.first;
            auto stack = kv.second;
            std::string color = "white";
            std::string topLine = "     "; // Lignes vides pour les hexes vides
            std::string botLine = "     ";

            if (!stack.empty()) {
                // Case occupée : récupérer la pièce du dessus
                auto topPiece = stack.top();
                int ownerId = topPiece->getOwner().getId();
                color = (ownerId == 1) ? "red" : "blue";

                // aficher l'id du joueur en topLine
                std::string pId = std::to_string(topPiece->getOwner().getId());
                topLine = "  " + pId + "  ";

                // Afficher uniquement la lettre de la pièce
                std::string pChar = pieceTypeToChar(topPiece->getType());
                botLine = "  " + pChar + "  ";
            }

            // Créer l'affichage de l'hexagone
            auto cellLines = drawHexCell(color, topLine, botLine);
            hexGrid[{h.getX(), h.getY()}] = cellLines;
        }

        // Afficher la grille
        const int HEX_HEIGHT = 6; // Hauteur d'un hexagone
        for (int y = maxY; y >= minY; --y) {
            int offsetSpaces = (y % 2 == 0) ? 0 : 5; // Décalage pour rangées impaires
            std::vector<std::string> rowLines(HEX_HEIGHT, std::string(offsetSpaces, ' '));

            for (int x = minX; x <= maxX; ++x) {
                auto it = hexGrid.find({x, y});
                if (it != hexGrid.end()) {
                    auto &cell = it->second;
                    for (int i = 0; i < HEX_HEIGHT; ++i) {
                        rowLines[i] += cell[i] + "  ";
                    }
                } else {
                    for (int i = 0; i < HEX_HEIGHT; ++i) {
                        rowLines[i] += std::string(11, ' '); // Case vide
                    }
                }
            }

            for (auto &line: rowLines) {
                std::cout << line << std::endl;
            }
        }

        std::cout << termcolor::reset;
    }
}

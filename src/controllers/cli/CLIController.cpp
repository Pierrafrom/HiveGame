#include "CLIController.h"
#include "models/Move.h"
#include "models/PieceFactory.h"
#include <stdexcept>
#include <iostream>
#include <models/GameRules.h>

namespace hive::controllers::cli {
    CLIController::CLIController(std::shared_ptr<views::cli::CLIView> view)
        : view(std::move(view)), game(models::Game::getInstance()) {
    }

    void CLIController::run() const {
        view->displayMessage("=== Bienvenue dans Hive (CLI) ===");
        const std::string p1 = view->askPlayerName(1);
        const std::string p2 = view->askPlayerName(2);
        game.resetGame(p1, p2);

        bool continueGame = true;
        while (continueGame) {
            displayGameState();

            if (const auto [isGameOver, isDraw, winner] = game.getGameStatus();
                isGameOver) {
                if (isDraw) {
                    view->displayMessage("La partie est terminée : match nul !");
                } else {
                    view->displayMessage("La partie est terminée, le gagnant est : " + winner->getName());
                }
                break;
            }

            std::string command = view->getUserCommand();
            continueGame = handleCommand(command);
        }

        view->displayMessage("Merci d’avoir joué !");
    }

    bool CLIController::handleCommand(const std::string &command) const {
        if (command == "place") {
            placePiece();
        } else if (command == "move") {
            movePiece();
        } else if (command == "undo") {
            undoMove();
        } else if (command == "redo") {
            redoMove();
        } else if (command == "quit") {
            return false;
        } else {
            view->displayError("Commande inconnue. Commandes possibles: place, move, undo, redo, quit.");
        }
        return true;
    }

    void CLIController::placePiece() const {
        try {
            const models::enums::PieceType chosenType = choosePieceType();
            const auto player = game.getPlayerPtr();

            // Création de la pièce via la PieceFactory (unique_ptr)
            auto pieceUnique = models::PieceFactory::createPiece(chosenType);
            const std::shared_ptr<models::Piece> pieceShared(pieceUnique.release());

            const auto &board = game.getBoard();
            std::vector<models::Hex> validPositions;

            // On parcourt toutes les hex vides pour trouver celles valides pour le placement
            for (const auto &hex: board.allEmptyHexes()) {
                models::Move testMove(player, pieceShared, hex);
                try {
                    models::GameRules::validateMove(testMove, board, game.getTurnNumber());
                    validPositions.push_back(hex);
                } catch (const std::exception &) {
                    // Ignorer les positions invalides
                }
            }

            if (validPositions.empty()) {
                throw std::runtime_error("Aucune position valide pour placer cette pièce.");
            }

            // Permettre au joueur de choisir une position valide
            const models::Hex targetHex = chooseTargetHexFromMoves(validPositions);

            // Création du move final (placement)
            const models::Move move(player, pieceShared, targetHex);
            game.executeMove(move);
        } catch (const std::exception &e) {
            view->displayError(e.what());
        }
    }

    void CLIController::movePiece() const {
        try {
            const auto &currentPlayer = game.getCurrentPlayer();
            const auto player = game.getPlayerPtr();
            const auto playerPieces = player->getPieces(); // Toutes les pièces du joueur

            // Préparer une liste des descriptions des pièces pour affichage
            std::vector<std::string> pieceDescriptions;
            pieceDescriptions.reserve(playerPieces.size());
            for (const auto &p: playerPieces) {
                pieceDescriptions.push_back(p->toString());
            }

            // Demander au joueur de sélectionner une pièce
            const int chosenIndex = view->askOption("Sélectionnez une pièce à déplacer :", pieceDescriptions);
            const std::shared_ptr<models::Piece>& chosenPiece = playerPieces[chosenIndex];

            // Récupérer la position de la pièce
            const models::Hex fromHex = chosenPiece->getPosition().value();

            // Obtenir les mouvements possibles pour la pièce
            const auto &strategy = chosenPiece->getMoveStrategy();
            auto possibleMoves = strategy.getPossibleMoves(game.getBoard(), currentPlayer);

            if (possibleMoves.empty()) {
                throw std::runtime_error("Cette pièce ne peut pas se déplacer. Veuillez choisir une autre pièce.");
            }

            // Filtrer les mouvements par les règles du jeu
            std::vector<models::Hex> validMoves;
            const auto &board = game.getBoard();
            for (const auto &dest: possibleMoves) {
                models::Move testMove(player, chosenPiece, fromHex, dest);
                try {
                    models::GameRules::validateMove(testMove, board, game.getTurnNumber());
                    validMoves.push_back(dest);
                } catch (const std::exception &) {
                    // Mouvement non valide, on l'ignore
                }
            }

            if (validMoves.empty()) {
                throw std::runtime_error("Aucun mouvement valide n’est disponible pour cette pièce.");
            }

            // Permettre au joueur de choisir une destination parmi les mouvements valides
            const models::Hex toHex = chooseTargetHexFromMoves(validMoves);

            // Exécuter le mouvement
            const models::Move move(player, chosenPiece, fromHex, toHex);
            game.executeMove(move);
        } catch (const std::exception &e) {
            view->displayError(e.what());
        }
    }


    void CLIController::undoMove() const {
        try {
            game.undo();
        } catch (const std::exception &e) {
            view->displayError(e.what());
        }
    }

    void CLIController::redoMove() const {
        try {
            game.redo();
        } catch (const std::exception &e) {
            view->displayError(e.what());
        }
    }

    models::enums::PieceType CLIController::choosePieceType() const {
        const std::vector<std::string> options = {
            "Queen Bee",
            "Ant",
            "Beetle",
            "Grasshopper",
            "Ladybug",
            "Mosquito",
            "Spider",
            "Pillbug"
        };

        switch (view->askOption("Choisissez le type de pièce à placer :", options)) {
            case 0: return models::enums::PieceType::QUEEN_BEE;
            case 1: return models::enums::PieceType::ANT;
            case 2: return models::enums::PieceType::BEETLE;
            case 3: return models::enums::PieceType::GRASSHOPPER;
            case 4: return models::enums::PieceType::LADYBUG;
            case 5: return models::enums::PieceType::MOSQUITO;
            case 6: return models::enums::PieceType::SPIDER;
            case 7: return models::enums::PieceType::PILLBUG;
            default:
                return models::enums::PieceType::QUEEN_BEE;
        }
    }

    models::Hex CLIController::chooseTargetHexFromMoves(const std::vector<models::Hex> &possibleMoves) const {
        std::vector<std::string> options;
        options.reserve(possibleMoves.size());
        for (const auto &hex: possibleMoves) {
            options.push_back(hex.toString());
        }

        const int choice = view->askOption("Choisissez une position valide :", options);
        return possibleMoves[choice];
    }
}

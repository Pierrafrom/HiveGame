/*#include "controllers/cli/CLIController.h"
#include "views/cli/CLIView.h"
#include <memory>

auto main() -> int {
    const auto view = std::make_shared<hive::views::cli::CLIView>();
    const hive::controllers::cli::CLIController controller(view);
    controller.run();
    return 0;
}*/

#include <gtest/gtest.h>
#include "models/Game.h"
#include "utils/Serializer.h"
#include <iostream>
#include "models/Move.h"
#include "models/Hex.h"
#include "models/Player.h"
#include "models/PieceFactory.h"

/*int main(int argc, char **argv) {
    hive::models::Game &game = hive::models::Game::getInstance();
    hive::utils::Serializer &serializer = hive::utils::Serializer::getInstance();
    std::string testFilename = "./test_game_state.json";

    game.resetGame("Alice", "Bob");

    bool saveResult = hive::utils::Serializer::saveGame(game);

    if (saveResult) {
        std::cout << "Game saved successfully!" << std::endl;
    } else {
        std::cerr << "Failed to data game!" << std::endl;
    }
}*/

int main(int argc, char **argv) {
    try {
        // Singleton Game instance
        hive::models::Game &game = hive::models::Game::getInstance();
        std::cout << "Initializing game with players: Alice and Bob\n" << std::endl;

        // Réinitialisation du jeu avec deux joueurs
        game.resetGame("Alice", "Bob");
        std::cout<<"JOUEUR 1:" << game.getPlayer(0) << std::endl;
        std::cout<<"JOUEUR 2:" << game.getPlayer(1) << std::endl;

        // Création de pièces via PieceFactory (conversion std::unique_ptr en std::shared_ptr)
        std::shared_ptr<hive::models::Piece> queenBeeAlice =
                std::move(hive::models::PieceFactory::createPiece(hive::models::enums::PieceType::QUEEN_BEE, 1));
        std::shared_ptr<hive::models::Piece> antBob =
                std::move(hive::models::PieceFactory::createPiece(hive::models::enums::PieceType::ANT, 2));


        // Positions hexagonales pour les mouvements
        hive::models::Hex pos1(0, 0, 0);
        hive::models::Hex pos2(1, -1, 0);
        hive::models::Hex pos3(2, -2, 0);


        // Exécution des mouvements
        std::cout << "Executing moves..." << std::endl;

        // Alice place QueenBee
        game.executeMove(hive::models::Move(game.getPlayerPtr(0), queenBeeAlice, pos1));
        std::cout << "Move 1: Alice places QueenBee at " << pos1 << std::endl;

        // Bob place Ant
        game.executeMove(hive::models::Move(game.getPlayerPtr(1), antBob, pos2));
        std::cout << "Move 2: Bob places Ant at " << pos2 << std::endl;

        // Alice déplace QueenBee
        game.executeMove(hive::models::Move(game.getPlayerPtr(0), queenBeeAlice, pos1, pos3));
        std::cout << "Move 3: Alice moves QueenBee from " << pos1 << " to " << pos3 << std::endl;

        // Afficher l'état actuel du jeu
        std::cout << "\nCurrent Game State:\n" << game << std::endl;

        // Simuler un undo
        std::cout << "Performing undo..." << std::endl;
        game.undo();
        std::cout << "Undo successful. Current Game State:\n" << game << std::endl;

        // Sauvegarde de l'état du jeu
        const std::string saveFile = "data/game_save_after_undo.json";
        std::cout << "Saving game state to file: " << saveFile << std::endl;
        bool saveResult = hive::utils::Serializer::saveGame(game, saveFile);
        if (saveResult) {
            std::cout << "Game saved successfully!" << std::endl;
        } else {
            std::cerr << "Failed to save game!" << std::endl;
        }

        // Simuler un redo
        std::cout << "Performing redo..." << std::endl;
        game.redo();
        std::cout << "Redo successful. Current Game State:\n" << game << std::endl;

        // Sauvegarde de l'état du jeu après redo
        const std::string saveFileAfterRedo = "data/game_save_after_redo.json";
        std::cout << "Saving game state to file: " << saveFileAfterRedo << std::endl;
        saveResult = hive::utils::Serializer::saveGame(game, saveFileAfterRedo);
        if (saveResult) {
            std::cout << "Game saved successfully!" << std::endl;
        } else {
            std::cerr << "Failed to save game!" << std::endl;
        }

        // Test de la fonction loadGame

        std::cout << "\nLoading game state from file: " << saveFileAfterRedo << std::endl;

        hive::models::Game &loadedGame = hive::models::Game::getInstance();
        bool loadResult = hive::utils::Serializer::loadGame(loadedGame, saveFileAfterRedo);

        if (loadResult) {
            std::cout << "Game loaded successfully! Loaded Game State:\n" << loadedGame << std::endl;
        } else {
            std::cerr << "Failed to load game!" << std::endl;
        }

        std::shared_ptr<hive::models::Piece> queenBeeBob =
                std::move(hive::models::PieceFactory::createPiece(hive::models::enums::PieceType::QUEEN_BEE, 2));
        hive::models::Hex pos4(1, 0, -1);

        // Exécution des mouvements
        std::cout << "Executing moves..." << std::endl;
        // Bob place QueenBee
        game.executeMove(hive::models::Move(game.getPlayerPtr(1), queenBeeBob, pos4));
        std::cout << "Move 4: Bob places QueenBee at " << pos1 << std::endl;

        // Sauvegarde de l'état du jeu après load
        const std::string saveFileAfterLoad = "data/game_save_after_load.json";
        std::cout << "Saving game state to file: " << saveFileAfterRedo << std::endl;
        saveResult = hive::utils::Serializer::saveGame(game, saveFileAfterLoad);
        if (saveResult) {
            std::cout << "Game saved successfully!" << std::endl;
        } else {
            std::cerr << "Failed to save game!" << std::endl;
        }





    } catch (const std::exception &e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }

    return 0;
}

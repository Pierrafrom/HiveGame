/**
 * @file GameTest.cpp
 * @brief Unit tests for the Game class in the Hive game project.
 *
 * This file contains tests for validating the Singleton pattern,
 * game initialization, and other behaviors of the Game class.
 */

#include <gtest/gtest.h>
#include <models/PieceFactory.h>

#include "models/Game.h"

namespace hive::models {
    /**
     * @class GameTest
     * @brief Test suite for the Game class.
     *
     * This class contains unit tests to validate the behavior of the Game class,
     * including Singleton implementation, game initialization, and deleted operations.
     */
    class GameTest : public testing::Test {
    protected:
        /**
         * @brief Optional setup before each test.
         */
        void SetUp() override {
            Game &game = Game::getInstance();
            game.resetGame();
        }

        /**
         * @brief Helper function to surround a queen bee on the board.
         * @param queenBeePosition The position of the queen bee to surround.
         */
        static void surroundQueenBee(const Hex &queenBeePosition) {
            Game &game = Game::getInstance();
            // Surround the queen bee with pieces
            for (const std::vector<Hex> surroundingHexes = game.getBoard().neighbors(queenBeePosition);
                 const auto &hex: surroundingHexes) {
                auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
                const std::shared_ptr pieceShared = std::move(piece);
                if (!game.getBoard().isOccupied(hex)) {
                    game.getBoard().addPiece(hex, pieceShared);
                    game.getCurrentPlayer().addPiece(pieceShared);
                }
            }
        }
    };

    /*******************************************************************************************************************
     * Game Singleton Tests
     ******************************************************************************************************************/

    /**
     * @test GameTest.SingletonInstanceIsSame
     * @brief Test that ensures the Singleton instance of the Game class is unique.
     *
     * This test validates that the `getInstance` method always returns the same instance.
     */
    TEST_F(GameTest, SingletonInstanceIsSame) {
        Game &instance1 = Game::getInstance();
        Game &instance2 = Game::getInstance();

        EXPECT_EQ(&instance1, &instance2) << "Singleton instances should be the same";
    }

    /**
     * @test GameTest.ResetGameInitializesCorrectly
     * @brief Test that ensures the game is correctly initialized after a reset.
     *
     * This test verifies that:
     * - The turn number is reset to 1.
     * - The first player is correctly set.
     * - The names of both players are initialized as expected.
     */
    TEST_F(GameTest, ResetGameInitializesCorrectly) {
        Game &game = Game::getInstance();

        // Reset the game with specific player names
        game.resetGame("Alice", "Bob");

        // Validate game state
        EXPECT_EQ(game.getTurnNumber(), 1) << "Turn number should be 1 after reset";
        EXPECT_EQ(game.getCurrentPlayer().getName(), "Alice") << "First player should be 'Alice'";
        EXPECT_EQ(game.getPlayer(0).getName(), "Alice") << "Player 0 should be 'Alice'";
        EXPECT_EQ(game.getPlayer(1).getName(), "Bob") << "Player 1 should be 'Bob'";
    }

    /**
     * @test GameTest.ResetGameClearsBoardAndStacks
     * @brief Test that ensures the board and undo/redo stacks are cleared after a reset.
     *
     * This test verifies that:
     * - The board is cleared and only contains the origin hex.
     * - Undo and redo stacks are empty, throwing exceptions when attempting to undo or redo.
     */
    TEST_F(GameTest, ResetGameClearsBoardAndStacks) {
        Game &game = Game::getInstance();

        // Perform some operations
        game.resetGame("Alice", "Bob");

        // Validate the board is cleared
        EXPECT_TRUE(game.getBoard().getBoard().size() == 1) << "Board should contain only the origin hex";

        // Ensure undo/redo stacks are empty
        EXPECT_THROW(game.undo(), std::runtime_error) << "Undo stack should be empty after reset";
        EXPECT_THROW(game.redo(), std::runtime_error) << "Redo stack should be empty after reset";
    }

    /**
     * @test GameTest.CopyConstructorDeleted
     * @brief Test that ensures the copy constructor is deleted.
     *
     * This test validates that the Game class cannot be copied.
     */
    TEST_F(GameTest, CopyConstructorDeleted) {
        static_assert(!std::is_copy_constructible_v<Game>,
                      "Game copy constructor should be deleted");
    }

    /**
     * @test GameTest.CopyAssignmentDeleted
     * @brief Test that ensures the copy assignment operator is deleted.
     *
     * This test validates that the Game class cannot be copy-assigned.
     */
    TEST_F(GameTest, CopyAssignmentDeleted) {
        static_assert(!std::is_copy_assignable_v<Game>,
                      "Game copy assignment operator should be deleted");
    }

    /**
     * @test GameTest.MoveConstructorDeleted
     * @brief Test that ensures the move constructor is deleted.
     *
     * This test validates that the Game class cannot be moved.
     */
    TEST_F(GameTest, MoveConstructorDeleted) {
        static_assert(!std::is_move_constructible_v<Game>,
                      "Game move constructor should be deleted");
    }

    /**
     * @test GameTest.MoveAssignmentDeleted
     * @brief Test that ensures the move assignment operator is deleted.
     *
     * This test validates that the Game class cannot be move-assigned.
     */
    TEST_F(GameTest, MoveAssignmentDeleted) {
        static_assert(!std::is_move_assignable_v<Game>,
                      "Game move assignment operator should be deleted");
    }

    /*******************************************************************************************************************
     * Getters Tests
     ******************************************************************************************************************/

    /**
     * @test GameTest.GetBoardReturnsValidBoard
     * @brief Test that ensures the game board is correctly returned by the `getBoard` method.
     *
     * This test validates that the `getBoard` method returns a valid board instance
     * and throws an exception if the board is not initialized.
     */
    TEST_F(GameTest, GetBoardReturnsValidBoard) {
        Game &game = Game::getInstance();

        // Ensure the board is valid after game initialization
        game.resetGame("Alice", "Bob");
        EXPECT_NO_THROW(game.getBoard()) << "getBoard should return a valid board instance.";
        EXPECT_EQ(game.getBoard().getBoard().size(), 1) << "Board should initially contain only the origin hex.";
    }

    /**
     * @test GameTest.GetBoardConstAndNonConst
     * @brief Test that ensures both const and non-const versions of `getBoard` work correctly.
     *
     * This test validates:
     * - The const version of `getBoard` provides a constant reference to the board.
     * - The non-const version of `getBoard` allows modifications to the board.
     */
    TEST_F(GameTest, GetBoardConstAndNonConst) {
        Game &game = Game::getInstance();

        // Reset the game to ensure a clean state
        game.resetGame("Alice", "Bob");

        // Test the const version
        const Game &constGame = game; // Reference to const Game
        const Board &constBoard = constGame.getBoard();
        EXPECT_EQ(constBoard.getBoard().size(), 1) << "Const board should initially contain only the origin hex.";

        // Test the non-const version
        Board &nonConstBoard = game.getBoard();
        auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr antShared = std::move(ant);
        const Hex position(0, 0, 0);
        nonConstBoard.addPiece(position, antShared);

        // Verify that the board has been modified through the non-const reference
        EXPECT_TRUE(nonConstBoard.isOccupied(position)) << "Non-const board should allow modifications.";
        EXPECT_EQ(game.getBoard().getBoard().size(),
                  7) << "Board should reflect changes made through the non-const reference.";

        // Verify that the const reference reflects the same modifications
        EXPECT_TRUE(
            constBoard.isOccupied(position)) << "Const board should reflect changes made to the non-const board.";
    }

    /**
     * @test GameTest.GetCurrentPlayerReturnsCorrectPlayer
     * @brief Test that ensures the `getCurrentPlayer` method returns the correct player.
     *
     * This test validates that the correct player is returned for the current turn.
     */
    TEST_F(GameTest, GetCurrentPlayerReturnsCorrectPlayer) {
        Game &game = Game::getInstance();

        // Reset the game with specific player names
        game.resetGame("Alice", "Bob");

        // Check the current player
        EXPECT_EQ(game.getCurrentPlayer().getName(), "Alice") << "Current player should initially be 'Alice'.";
        game.nextTurn();
        EXPECT_EQ(game.getCurrentPlayer().getName(), "Bob") << "Current player should switch to 'Bob'.";
    }

    /**
     * @test GameTest.GetPlayerByIndex
     * @brief Test that ensures the `getPlayer` method returns the correct player for a valid index.
     *
     * This test validates that the players are correctly accessible by index.
     */
    TEST_F(GameTest, GetPlayerByIndex) {
        Game &game = Game::getInstance();

        // Reset the game with specific player names
        game.resetGame("Alice", "Bob");

        // Validate players by index
        EXPECT_EQ(game.getPlayer(0).getName(), "Alice") << "Player at index 0 should be 'Alice'.";
        EXPECT_EQ(game.getPlayer(1).getName(), "Bob") << "Player at index 1 should be 'Bob'.";
    }

    /**
     * @test GameTest.GetPlayerInvalidIndexThrows
     * @brief Test that ensures `getPlayer` throws an exception for an invalid index.
     *
     * This test verifies that accessing an out-of-range player index throws the expected exception.
     */
    TEST_F(GameTest, GetPlayerInvalidIndexThrows) {
        Game &game = Game::getInstance();

        // Reset the game
        game.resetGame("Alice", "Bob");

        // Access invalid index
        EXPECT_THROW(game.getPlayer(2),
                     std::out_of_range) << "Accessing invalid player index should throw an exception.";
        EXPECT_THROW(game.getPlayer(100),
                     std::out_of_range) << "Accessing a high invalid index should throw an exception.";
    }

    /**
     * @test GameTest.GetTurnNumberReturnsCorrectValue
     * @brief Test that ensures the `getTurnNumber` method returns the correct turn number.
     *
     * This test validates that the turn number starts at 1 and increments correctly.
     */
    TEST_F(GameTest, GetTurnNumberReturnsCorrectValue) {
        Game &game = Game::getInstance();

        // Reset the game
        game.resetGame("Alice", "Bob");

        // Validate initial turn number
        EXPECT_EQ(game.getTurnNumber(), 1) << "Turn number should initially be 1.";

        // Simulate advancing turns
        game.nextTurn();
        EXPECT_EQ(game.getTurnNumber(), 2) << "Turn number should increment to 2 after nextTurn.";
    }

    /*******************************************************************************************************************
     * Game Logic Tests
     ******************************************************************************************************************/

    /**
     * @test GameTest.GetGameStatusReturnsCorrectState
     * @brief Test that ensures `getGameStatus` correctly identifies the game status.
     */
    TEST_F(GameTest, GetGameStatusReturnsCorrectState) {
        Game &game = Game::getInstance();
        auto queenBee1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto queenBee2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr queenBee1Shared = std::move(queenBee1);
        const std::shared_ptr queenBee2Shared = std::move(queenBee2);
        game.getPlayer(0).addPiece(queenBee1Shared);
        game.getPlayer(1).addPiece(queenBee2Shared);
        game.getBoard().addPiece(Hex(0, 0, 0), queenBee1Shared);

        auto status = game.getGameStatus();
        EXPECT_FALSE(status.isGameOver) << "Game should not be over if both queen bees are not surrounded.";
        ASSERT_EQ(status.winner, nullptr) << "Game should not have a winner if both queen bees are not surrounded.";
        EXPECT_FALSE(status.isDraw) << "Game should not be a draw if both queen bees are not surrounded.";

        game.getBoard().addPiece(Hex(1, -1, 0), queenBee2Shared);

        status = game.getGameStatus();
        EXPECT_FALSE(status.isGameOver) << "Game should not be over if both queen bees are not surrounded.";
        ASSERT_EQ(status.winner, nullptr) << "Game should not have a winner if both queen bees are not surrounded.";
        EXPECT_FALSE(status.isDraw) << "Game should not be a draw if both queen bees are not surrounded.";

        game.nextTurn();
        surroundQueenBee(Hex(0, 0, 0));
        status = game.getGameStatus();
        EXPECT_TRUE(status.isGameOver) << "Game should be over if one queen bee is surrounded.";
        ASSERT_EQ(status.winner, &game.getPlayer(1)) << "Player 1 should win if one queen bee is surrounded.";
        EXPECT_FALSE(status.isDraw) << "Game should not be a draw if one queen bee is surrounded.";

        surroundQueenBee(Hex(1, -1, 0));
        status = game.getGameStatus();
        EXPECT_TRUE(status.isGameOver) << "Game should be over if both queen bees are surrounded.";
        ASSERT_EQ(status.winner, nullptr) << "Game should be a draw if both queen bees are surrounded.";
        EXPECT_TRUE(status.isDraw) << "Game should be a draw if both queen bees are surrounded.";
    }

    TEST_F(GameTest, ExecuteMovePushesToUndoStackAndClearsRedoStack) {
        Game &game = Game::getInstance();
        game.resetGame("Alice", "Bob");

        // Créer la pièce et définir la destination
        auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex to(0, 0, 0);

        // Créer le mouvement
        const Move move(game.getPlayerPtr(), sharedPiece, to);

        // Vérifier que les piles sont vides au départ
        EXPECT_TRUE(game.getUndoStack().empty()) << "Undo stack should be empty before move.";
        EXPECT_TRUE(game.getRedoStack().empty()) << "Redo stack should be empty before move.";

        // Exécuter le mouvement
        game.executeMove(move);

        // Vérifier que le mouvement a été ajouté à la pile undo
        EXPECT_FALSE(game.getUndoStack().empty()) << "Undo stack should contain the move after execution.";
        EXPECT_EQ(game.getUndoStack().size(), 1) << "Undo stack should contain one move.";

        // Vérifier que la pile redo a été vidée après l'exécution d'un nouveau mouvement
        EXPECT_TRUE(game.getRedoStack().empty()) << "Redo stack should be empty after a new move.";
    }

    TEST_F(GameTest, UndoMovesToRedoStackAndRevertsBoard) {
        Game &game = Game::getInstance();
        game.resetGame("Alice", "Bob");

        // Créer la pièce et définir la destination
        auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex to(0, 0, 0);

        // Créer et exécuter le mouvement
        const Move move(game.getPlayerPtr(), sharedPiece, to);
        game.executeMove(move);

        // Vérifier que le mouvement est dans la pile undo
        EXPECT_FALSE(game.getUndoStack().empty()) << "Undo stack should contain the move after execution.";

        // Annuler le mouvement
        game.undo();

        // Vérifier que la pile redo contient maintenant le mouvement
        EXPECT_FALSE(game.getRedoStack().empty()) << "Redo stack should contain the move after undo.";
        EXPECT_EQ(game.getRedoStack().size(), 1) << "Redo stack should contain one move.";

        // Vérifier que la pile undo est vide après l'annulation
        EXPECT_TRUE(game.getUndoStack().empty()) << "Undo stack should be empty after undo.";

        // Vérifier l'état du plateau (la pièce devrait avoir été enlevée)
        EXPECT_FALSE(game.getBoard().isOccupied(to)) << "Piece should be removed after undoing the move.";
    }

    TEST_F(GameTest, RedoReappliesMoveAndMovesToUndoStack) {
        Game &game = Game::getInstance();
        game.resetGame("Alice", "Bob");

        // Créer la pièce et définir la destination
        auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex to(0, 0, 0);

        // Créer et exécuter le mouvement
        const Move move(game.getPlayerPtr(), sharedPiece, to);
        game.executeMove(move);

        // Annuler le mouvement
        game.undo();

        // Vérifier que la pile redo contient maintenant le mouvement
        EXPECT_FALSE(game.getRedoStack().empty()) << "Redo stack should contain the move after undo.";

        // Réappliquer le mouvement
        game.redo();

        // Vérifier que la pile undo contient maintenant le mouvement
        EXPECT_FALSE(game.getUndoStack().empty()) << "Undo stack should contain the move after redo.";

        // Vérifier que la pile redo est vide après un redo
        EXPECT_TRUE(game.getRedoStack().empty()) << "Redo stack should be empty after redo.";

        // Vérifier l'état du plateau (la pièce devrait avoir été replacée)
        EXPECT_TRUE(game.getBoard().isOccupied(to)) << "Piece should be placed back on the board after redo.";
    }

    TEST_F(GameTest, UndoThrowsExceptionIfUndoStackIsEmpty) {
        Game &game = Game::getInstance();
        game.resetGame("Alice", "Bob");

        // Vérifier que la pile undo est vide
        EXPECT_TRUE(game.getUndoStack().empty()) << "Undo stack should be empty before any moves.";

        // Essayer de faire un undo quand la pile undo est vide
        EXPECT_THROW({
                     game.undo();
                     }, std::runtime_error) << "Undo should throw an exception if undo stack is empty.";
    }

    TEST_F(GameTest, RedoThrowsExceptionIfRedoStackIsEmpty) {
        Game &game = Game::getInstance();
        game.resetGame("Alice", "Bob");

        // Vérifier que la pile redo est vide
        EXPECT_TRUE(game.getRedoStack().empty()) << "Redo stack should be empty before any moves.";

        // Essayer de faire un redo quand la pile redo est vide
        EXPECT_THROW({
                     game.redo();
                     }, std::runtime_error) << "Redo should throw an exception if redo stack is empty.";
    }

    TEST_F(GameTest, NextTurnAdvancesToNextPlayer) {
        Game &game = Game::getInstance();
        game.resetGame("Alice", "Bob");

        // Vérifier que le joueur actuel est Alice au début
        EXPECT_EQ(game.getCurrentPlayer().getName(), "Alice") << "The first player should be Alice.";

        // Passer au tour suivant
        game.nextTurn();

        // Vérifier que le joueur actuel est Bob après avoir avancé au tour suivant
        EXPECT_EQ(game.getCurrentPlayer().getName(), "Bob") << "After nextTurn, the current player should be Bob.";
    }

    TEST_F(GameTest, ToStringReturnsCorrectGameState) {
        Game &game = Game::getInstance();
        game.resetGame("Alice", "Bob");

        // Appeler la méthode toString et vérifier qu'elle contient des informations sur le tour et les joueurs
        const std::string gameState = game.toString();

        // Vérifier que le nom du joueur et le numéro de tour sont présents dans la chaîne
        EXPECT_TRUE(
            gameState.find("Turn Number: 1") != std::string::npos) << "Turn number should be in the game state.";
        EXPECT_TRUE(gameState.find("Alice") != std::string::npos) << "Player 'Alice' should be in the game state.";
        EXPECT_TRUE(gameState.find("Bob") != std::string::npos) << "Player 'Bob' should be in the game state.";
    }

    TEST_F(GameTest, OperatorStreamOutputsCorrectGameState) {
        Game &game = Game::getInstance();
        game.resetGame("Alice", "Bob");

        std::stringstream ss;
        ss << game;

        const std::string output = ss.str();
        EXPECT_TRUE(output.find("Turn Number: 1") != std::string::npos) << "Turn number should be in the output.";
        EXPECT_TRUE(output.find("Alice") != std::string::npos) << "Player 'Alice' should be in the output.";
        EXPECT_TRUE(output.find("Bob") != std::string::npos) << "Player 'Bob' should be in the output.";
    }
} // namespace hive::models

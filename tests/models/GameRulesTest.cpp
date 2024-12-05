// GameRulesTest.cpp
#include <config/constants.h>
#include <gtest/gtest.h>
#include <models/Board.h>
#include <models/GameRules.h>
#include <models/PieceFactory.h>


namespace hive::models {
    class GameRulesTest : public testing::Test {
    protected:
        // TODO: declare any variables you need here
        Board board;  /**< The game board used for tests */
        Player player; /**< A player instance */
        GameRules &gameRules = GameRules::getInstance(); /**< Singleton instance of GameRules */
        std::shared_ptr<Player> player2; /**< Shared pointer to the player performing the move */
        std::shared_ptr<Player> opponent;  /**< Opponent for testing ownership */

        // Setup method for initializing the player
        void SetUp() override {
            //TODO : initialize any variables you need here
            // every test will run this function before executing
            // Initialize player and other objects
            player = Player(1, "Player1"); // Assuming a Player constructor that takes a name and ID
            player2 = std::make_shared<Player>("Player2");
            opponent = std::make_shared<Player>("Player3");

        }

        // TearDown method for cleaning up
        void TearDown() override {
            //TODO : clean up any variables you need here
            // every test will run this function after executing
            board.clear();
        }
    };

    /**
     * @test SingletonPattern_ReturnsSameInstance
     * @brief Verifies that `getInstance` always returns the same instance.
     *
     * Ensures that multiple calls to `GameRules::getInstance()` point to the same
     * object in memory, validating the singleton pattern.
     */
    TEST_F(GameRulesTest, SingletonPattern_ReturnsSameInstance) {
        GameRules &instance1 = GameRules::getInstance();
        GameRules &instance2 = GameRules::getInstance();

        // Assert that the two instances are the same
        EXPECT_EQ(&instance1, &instance2);
    }

    /**
     * @test ValidateMove_ThrowsForUnoccupiedStartingHex
     * @brief Ensures `validateMove` throws an exception for a move starting on an unoccupied hex.
     */
    TEST_F(GameRulesTest, ValidateMove_ThrowsForUnoccupiedStartingHex) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        // Move without placing the piece on the board
        Move move(player2, sharedPiece, from, to);

        EXPECT_THROW({
            gameRules.validateMove(move, board, 1);
        }, std::invalid_argument);
    }

    /**
     * @test ValidateMove_ThrowsForUnownedPiece
     * @brief Ensures `validateMove` throws if the piece does not belong to the player.
     */
    TEST_F(GameRulesTest, ValidateMove_ThrowsForUnownedPiece) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);

        // Place the piece owned by the opponent
        board.addPiece(Hex(0, 0, 0), sharedPiece);
        sharedPiece->setOwner(opponent);

        Move move(player2, sharedPiece, Hex(0, 0, 0), Hex(1, -1, 0));

        EXPECT_THROW({
            gameRules.validateMove(move, board, 1);
        }, std::invalid_argument);
    }

    /**
     * @test ValidateMove_ThrowsForOccupiedTargetHex
     * @brief Ensures `validateMove` throws if the target hex is occupied by a non-beetle piece.
     */
    TEST_F(GameRulesTest, ValidateMove_ThrowsForOccupiedTargetHex) {
        auto movingPiece = PieceFactory::createPiece(enums::PieceType::ANT);
        auto targetPiece = PieceFactory::createPiece(enums::PieceType::SPIDER);

        const std::shared_ptr movingShared = std::move(movingPiece);
        const std::shared_ptr targetShared = std::move(targetPiece);

        board.addPiece(Hex(0, 0, 0), movingShared);
        movingShared->setOwner(player2);
        board.addPiece(Hex(1, -1, 0), targetShared);

        Move move(player2, movingShared, Hex(0, 0, 0), Hex(1, -1, 0));

        EXPECT_THROW({
            gameRules.validateMove(move, board, 1);
        }, std::invalid_argument);
    }

    /**
     * @test ValidateMove_AllowsBeetleToMoveToOccupiedHex
     * @brief Ensures that a beetle can move to an occupied hex.
     */
    TEST_F(GameRulesTest, ValidateMove_AllowsBeetleToMoveToOccupiedHex) {
        auto beetle = PieceFactory::createPiece(enums::PieceType::BEETLE);
        auto targetPiece = PieceFactory::createPiece(enums::PieceType::ANT);

        const std::shared_ptr beetleShared = std::move(beetle);
        beetleShared->setOwner(player2);
        const std::shared_ptr targetShared = std::move(targetPiece);
        targetShared->setOwner(opponent);

        board.addPiece(Hex(0, 0, 0), beetleShared);
        board.addPiece(Hex(1, -1, 0), targetShared);

        Move move(player2, beetleShared, Hex(0, 0, 0), Hex(1, -1, 0));

        EXPECT_NO_THROW({
            gameRules.validateMove(move, board, 1);
        });
    }

    /**
     * @test ValidateMove_AllowsValidMove
     * @brief Ensures `validateMove` allows a valid move scenario.
     */
    TEST_F(GameRulesTest, ValidateMove_AllowsValidMove) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);
        sharedPiece->setOwner(player2);

        board.addPiece(Hex(0, 0, 0), sharedPiece);

        Move move(player2, sharedPiece, Hex(0, 0, 0), Hex(1, -1, 0));

        EXPECT_NO_THROW({
            gameRules.validateMove(move, board, 1);
        });
    }
    /**
     * @test ValidateMove_ThrowsForNullPlayer
     * @brief Ensures `validateMove` throws a `std::invalid_argument` when the player is null.
     */
    TEST_F(GameRulesTest, ValidateMove_ThrowsForNullPlayer) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex to(0, 0, 0);

        Move move(nullptr, sharedPiece, to);

        EXPECT_THROW({
            gameRules.validateMove(move, board, 1);
        }, std::invalid_argument);
    }
    /**
     * @test ValidateMove_ThrowsForNullPiece
     * @brief Ensures `validateMove` throws a `std::invalid_argument` when the piece is null.
     */
    TEST_F(GameRulesTest, ValidateMove_ThrowsForNullPiece) {
        const Hex to(0, 0, 0);

        Move move(player2, nullptr, to);

        EXPECT_THROW({
            gameRules.validateMove(move, board, 1);
        }, std::invalid_argument);
    }

    /**
     * @test ValidateMove_ThrowsForQueenBeePlacementRequirement
     * @brief Ensures `validateMove` throws a `std::invalid_argument` when a queen bee placement is required, but a different piece is placed.
     */
    TEST_F(GameRulesTest, ValidateMove_ThrowsForQueenBeePlacementRequirement) {
        auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr antShared = std::move(ant);
        const Hex to(0, 0, 0);

        Move move(player2, antShared, to);

        // Simulate a turn where the queen bee must be placed
        size_t turnNumber = config::constants::MAX_TURN_BEFORE_QUEEN_PLACEMENT;

        EXPECT_THROW({
            gameRules.validateMove(move, board, turnNumber);
        }, std::invalid_argument);
    }

    /**
   * @test ValidateMove_ThrowsWhenPlacementNotAdjacent
   * @brief Ensures `validateMove` throws when the placement hex is not adjacent to existing pieces.
   */
    TEST_F(GameRulesTest, ValidateMove_ThrowsWhenPlacementNotAdjacent) {
        auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr antShared = std::move(ant);
        const Hex to(2, 2, -4); // A distant hex not adjacent to any pieces

        Move move(player2, antShared, to);

        EXPECT_THROW({
            gameRules.validateMove(move, board, 1);
        }, std::invalid_argument);
    }

    //==================================================
    //================== ALED ==========================
    //==================================================
    /**
     * @test ValidateMove_ThrowsWhenExceedingPieceLimit
     * @brief Ensures `validateMove` throws when a player tries to place more pieces than allowed of a specific type.
     */
    TEST_F(GameRulesTest, ValidateMove_ThrowsWhenExceedingPieceLimit) {
        auto queen = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto queen2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr queenShared = std::move(queen);
        const std::shared_ptr queenShared2 = std::move(queen2);
        const Hex to(0, 0, 0);

        queen2->setOwner(player2);
        board.addPiece(Hex(0, 0, 1), queenShared2);



        Move move(player2, queenShared, to);

        EXPECT_THROW({
            gameRules.validateMove(move, board, 1);
        }, std::invalid_argument);
    }

    //==================================================
    //================== ALED ==========================
    //==================================================
    /**
     * @test ValidateMove_ThrowsWhenPlacementAdjacentToOpponentPiece
     * @brief Ensures `validateMove` throws when the placement hex is adjacent to an opponent's piece.
     */
    TEST_F(GameRulesTest, ValidateMove_ThrowsWhenPlacementAdjacentToOpponentPiece) {
        auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        auto opponentPiece = PieceFactory::createPiece(enums::PieceType::SPIDER);

        const std::shared_ptr antShared = std::move(ant);
        const std::shared_ptr opponentShared = std::move(opponentPiece);

        board.addPiece(Hex(1, 0, -1), opponentShared); // Place opponent's piece near the target hex

        Move move(player2, antShared, Hex(0, 0, 0));

        EXPECT_THROW({
            gameRules.validateMove(move, board, 1);
        }, std::invalid_argument);
    }

    //==================================================
    //================== ALED ==========================
    //==================================================
    /**
 * @test ValidateMove_AllowsValidPlaceMove
 * @brief Ensures `validateMove` allows a valid PLACE move.
 */
    TEST_F(GameRulesTest, ValidateMove_AllowsValidPlaceMove) {
        auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr antShared = std::move(ant);

        // Place a piece belonging to the player to establish adjacency
        auto playerPiece = PieceFactory::createPiece(enums::PieceType::SPIDER);
        const std::shared_ptr playerShared = std::move(playerPiece);
        board.addPiece(Hex(1, 0, -1), playerShared);

        Move move(player2, antShared, Hex(0, 0, 0));

        EXPECT_NO_THROW({
            gameRules.validateMove(move, board, 1);
        });
    }



    // TODO: add your test cases here
}

// GameRulesTest.cpp
#include <config/constants.h>
#include <gtest/gtest.h>
#include <models/Board.h>
#include <models/GameRules.h>
#include <models/PieceFactory.h>


namespace hive::models {
    class GameRulesTest : public testing::Test {
    protected:
        Board board; /**< The game board used for tests */
        GameRules &gameRules = GameRules::getInstance(); /**< Singleton instance of GameRules */
        std::shared_ptr<Player> player; /**< Shared pointer to the player performing the move */
        std::shared_ptr<Player> opponent; /**< Opponent for testing ownership */

        // Setup method for initializing the player
        void SetUp() override {
            player = std::make_shared<Player>(1, "Player");
            opponent = std::make_shared<Player>(2, "Opponent");
        }

        // TearDown method for cleaning up
        void TearDown() override {
            board.clear();
        }

        /**
         * @brief Helper function to surround a queen bee on the board.
         * @param queenBeePosition The position of the queen bee to surround.
         */
        void surroundQueenBee(const Hex &queenBeePosition) {
            // Surround the queen bee with pieces
            for (const std::vector<Hex> surroundingHexes = board.neighbors(queenBeePosition);
                 const auto &hex: surroundingHexes) {
                auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
                const std::shared_ptr pieceShared = std::move(piece);
                if (!board.isOccupied(hex)) {
                    board.addPiece(hex, pieceShared);
                    player->addPiece(pieceShared);
                }
            }
        }
    };

    /*******************************************************************************************************************
     * Singleton Pattern Tests
     ******************************************************************************************************************/

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
     * @test GameRulesTest.CopyConstructorDeleted
     * @brief Test that ensures the copy constructor is deleted.
     *
     * This test validates that the Game class cannot be copied.
     */
    TEST_F(GameRulesTest, CopyConstructorDeleted) {
        static_assert(!std::is_copy_constructible_v<GameRules>,
                      "GameRules copy constructor should be deleted");
    }

    /**
     * @test GameRulesTest.CopyAssignmentDeleted
     * @brief Test that ensures the copy assignment operator is deleted.
     *
     * This test validates that the Game class cannot be copy-assigned.
     */
    TEST_F(GameRulesTest, CopyAssignmentDeleted) {
        static_assert(!std::is_copy_assignable_v<GameRules>,
                      "GameRules copy assignment operator should be deleted");
    }

    /**
     * @test GameRulesTest.MoveConstructorDeleted
     * @brief Test that ensures the move constructor is deleted.
     *
     * This test validates that the Game class cannot be moved.
     */
    TEST_F(GameRulesTest, MoveConstructorDeleted) {
        static_assert(!std::is_move_constructible_v<GameRules>,
                      "GameRules move constructor should be deleted");
    }

    /**
     * @test GameRulesTest.MoveAssignmentDeleted
     * @brief Test that ensures the move assignment operator is deleted.
     *
     * This test validates that the Game class cannot be move-assigned.
     */
    TEST_F(GameRulesTest, MoveAssignmentDeleted) {
        static_assert(!std::is_move_assignable_v<GameRules>,
                      "GameRules move assignment operator should be deleted");
    }

    /*******************************************************************************************************************
     * validateMove Tests
     ******************************************************************************************************************/

    /**
     * @test ValidateMove_ThrowsForQueenBeePlacementRequirement
     * @brief Ensures `validateMove` throws a `std::invalid_argument` when a queen bee placement is required, but a different piece is placed.
     */
    TEST_F(GameRulesTest, ValidateMove_ThrowsForQueenBeePlacementRequirement) {
        auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr antShared = std::move(ant);
        const Hex to(0, 0, 0);

        const Move move(player, antShared, to);

        // Simulate a turn where the queen bee must be placed
        constexpr size_t turnNumber = config::constants::MAX_TURN_BEFORE_QUEEN_PLACEMENT;

        EXPECT_THROW({
                     gameRules.validateMove(move, board, turnNumber);
                     }, std::runtime_error);
    }

    /**
    * @test ValidateMove_AllowsValidPlacement
    * @brief Ensures `validateMove` allows a valid piece placement and movement.
    */
    TEST_F(GameRulesTest, ValidateMove_AllowsValidPlacement) {
        auto bee = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr antShared = std::move(ant);
        const std::shared_ptr beeShared = std::move(bee);
        const Hex origin(0, 0, 0);

        const Move move1(player, beeShared, origin);

        EXPECT_NO_THROW({
            gameRules.validateMove(move1, board, 1);
            });

        move1.execute(board);

        const Move move2(opponent, antShared, board.neighbor(origin, enums::Direction::NORTH_EAST));

        EXPECT_NO_THROW({
            gameRules.validateMove(move2, board, 1);
            });

        move2.execute(board);

        const Move move3(player, beeShared, (beeShared->getPosition().value()),
                         board.neighbor(origin, enums::Direction::EAST));

        EXPECT_NO_THROW({
            gameRules.validateMove(move3, board, 1);
            });
    }

    /*******************************************************************************************************************
     * validateMoveType Tests
     ******************************************************************************************************************/

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
        const Move move(player, sharedPiece, from, to);

        EXPECT_THROW({
                     gameRules.validateMove(move, board, 1);
                     }, std::runtime_error);
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
        opponent->addPiece(sharedPiece);

        const Move move(player, sharedPiece, Hex(0, 0, 0), Hex(1, -1, 0));

        EXPECT_THROW({
                     gameRules.validateMove(move, board, 1);
                     }, std::runtime_error);
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
        player->addPiece(movingShared);
        board.addPiece(Hex(1, -1, 0), targetShared);

        const Move move(player, movingShared, Hex(0, 0, 0), Hex(1, -1, 0));

        EXPECT_THROW({
                     gameRules.validateMove(move, board, 1);
                     }, std::runtime_error);
    }

    /**
     * @test ValidateMove_AllowsBeetleToMoveToOccupiedHex
     * @brief Ensures that a beetle can move to an occupied hex.
     */
    TEST_F(GameRulesTest, ValidateMove_AllowsBeetleToMoveToOccupiedHex) {
        auto beetle = PieceFactory::createPiece(enums::PieceType::BEETLE);
        auto targetPiece = PieceFactory::createPiece(enums::PieceType::ANT);

        const std::shared_ptr beetleShared = std::move(beetle);
        player->addPiece(beetleShared);
        const std::shared_ptr targetShared = std::move(targetPiece);
        opponent->addPiece(targetShared);

        board.addPiece(Hex(0, 0, 0), beetleShared);
        board.addPiece(Hex(1, -1, 0), targetShared);

        const Move move(player, beetleShared, Hex(0, 0, 0), Hex(1, -1, 0));

        EXPECT_NO_THROW({
            gameRules.validateMove(move, board, 1);
            });
    }

    /*******************************************************************************************************************
     * validatePlaceType Tests
     ******************************************************************************************************************/

    /**
     * @test ValidateMove_ThrowsWhenExceedingPieceLimit
     * @brief Ensures `validateMove` throws when a player tries to place more pieces than allowed of a specific type.
     */
    TEST_F(GameRulesTest, ValidateMove_ThrowsWhenExceedingPieceLimit) {
        auto queen = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto queen2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);

        auto ant1 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto ant2 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto ant3 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto ant4 = PieceFactory::createPiece(enums::PieceType::ANT);

        const std::shared_ptr queenShared = std::move(queen);
        const std::shared_ptr queenShared2 = std::move(queen2);
        const std::shared_ptr antShared1 = std::move(ant1);
        const std::shared_ptr antShared2 = std::move(ant2);
        const std::shared_ptr antShared3 = std::move(ant3);
        const std::shared_ptr antShared4 = std::move(ant4);

        const Hex to(0, 0, 0);

        const Move move(player, queenShared, to);
        move.execute(board);
        const Move move2(player, queenShared2, board.neighbor(to, enums::Direction::SOUTH_EAST));
        EXPECT_THROW({
                     gameRules.validateMove(move2, board, 2);
                     }, std::runtime_error);

        const Move move3(player, antShared1, board.neighbor(to, enums::Direction::NORTH_EAST));
        move3.execute(board);
        const Move move4(player, antShared2, board.neighbor(to, enums::Direction::SOUTH_EAST));
        move4.execute(board);
        const Move move5(player, antShared3, board.neighbor(to, enums::Direction::NORTH_WEST));
        move5.execute(board);
        const Move move6(player, antShared4, board.neighbor(to, enums::Direction::SOUTH_WEST));
        EXPECT_THROW({
                     gameRules.validateMove(move6, board, 8);
                     }, std::runtime_error);
    }

    /**
     * @test ValidateMove_ThrowsWhenTargetHexOccupied
     * @brief Ensures `validateMove` throws when the target hex is already occupied.
     */
    TEST_F(GameRulesTest, ValidateMove_ThrowsWhenTargetHexOccupied) {
        auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        auto ant2 = PieceFactory::createPiece(enums::PieceType::ANT);

        const std::shared_ptr antShared = std::move(ant);
        const std::shared_ptr antShared2 = std::move(ant2);

        board.addPiece(Hex(0, 0, 0), antShared);
        player->addPiece(antShared);

        board.addPiece(Hex(1, -1, 0), antShared2);

        const Move move(player, antShared, Hex(1, -1, 0));

        EXPECT_THROW({
                     gameRules.validateMove(move, board, 1);
                     }, std::runtime_error);
    }

    /**
     * @test ValidateMove_ThrowsWhenAdjacentToOpponentPiece
     * @brief Ensures `validateMove` throws when the placement hex is adjacent to an opponent's piece.
     */
    TEST_F(GameRulesTest, ValidateMove_ThrowsWhenAdjacentToOpponentPiece) {
        auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        auto opponentPiece = PieceFactory::createPiece(enums::PieceType::SPIDER);
        auto ant2 = PieceFactory::createPiece(enums::PieceType::ANT);
        const Hex origin(0, 0, 0);
        const std::shared_ptr antShared = std::move(ant);
        const std::shared_ptr opponentShared = std::move(opponentPiece);
        const std::shared_ptr antShared2 = std::move(ant2);

        player->addPiece(antShared);
        opponent->addPiece(opponentShared);
        player->addPiece(antShared2);

        board.addPiece(origin, antShared);
        board.addPiece(board.neighbor(origin, enums::Direction::EAST), opponentShared);

        const Move move(player, antShared2,
                        board.neighbor(opponentShared->getPosition().value(), enums::Direction::NORTH_EAST));

        EXPECT_THROW({
                     gameRules.validateMove(move, board, 1);
                     }, std::runtime_error);
    }

    /*******************************************************************************************************************
     * getVictoryCondition Tests
     ******************************************************************************************************************/

    /**
     * @test VictoryCondition_NoQueenBee
     * @brief Ensures that the game continues if no queen bees are present on the board.
     */
    TEST_F(GameRulesTest, VictoryCondition_NoQueenBee) {
        const std::vector<const Player *> players = {player.get(), opponent.get()};

        const auto result = GameRules::getVictoryCondition(board, players);

        EXPECT_EQ(result, nullptr) << "The game should continue when no queen bees are on the board.";
    }

    /**
     * @test VictoryCondition_OneQueenBeeSurrounded
     * @brief Ensures that the player whose queen bee is surrounded loses, and the other player wins.
     */
    TEST_F(GameRulesTest, VictoryCondition_OneQueenBeeSurrounded) {
        // Place Player 1's queen bee
        auto queenBee1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr queenBee1Shared = std::move(queenBee1);
        player->addPiece(queenBee1Shared);
        board.addPiece(Hex(0, 0, 0), queenBee1Shared);

        // Surround Player 1's queen bee
        surroundQueenBee(Hex(0, 0, 0));

        // Add Player 2's queen bee (not surrounded)
        auto queenBee2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr queenBee2Shared = std::move(queenBee2);
        opponent->addPiece(queenBee2Shared);
        board.addPiece(Hex(1, -1, 0), queenBee2Shared);

        const std::vector<const Player *> players = {player.get(), opponent.get()};

        const auto result = GameRules::getVictoryCondition(board, players);

        EXPECT_EQ(result.value(), opponent.get()) << "Player 2 should win when Player 1's queen bee is surrounded.";
    }

    /**
     * @test VictoryCondition_Draw
     * @brief Ensures that the game results in a draw if all players' queen bees are surrounded.
     */
    TEST_F(GameRulesTest, VictoryCondition_Draw) {
        // Place and surround Player 1's queen bee
        auto queenBee1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr queenBee1Shared = std::move(queenBee1);
        player->addPiece(queenBee1Shared);
        board.addPiece(Hex(0, 0, 0), queenBee1Shared);
        surroundQueenBee(Hex(0, 0, 0));

        // Place and surround Player 2's queen bee
        auto queenBee2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr queenBee2Shared = std::move(queenBee2);
        opponent->addPiece(queenBee2Shared);
        board.addPiece(Hex(1, -1, 0), queenBee2Shared);
        surroundQueenBee(Hex(1, -1, 0));

        const std::vector<const Player *> players = {player.get(), opponent.get()};

        const auto result = GameRules::getVictoryCondition(board, players);

        EXPECT_EQ(result, std::nullopt) << "The game should result in a draw if all queen bees are surrounded.";
    }

    /**
     * @test VictoryCondition_QueenBeeNotPlaced
     * @brief Ensures that the game continues if one or both queen bees are not placed.
     */
    TEST_F(GameRulesTest, VictoryCondition_QueenBeeNotPlaced) {
        // Add Player 1's queen bee without placing it
        auto queenBee1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr queenBee1Shared = std::move(queenBee1);
        player->addPiece(queenBee1Shared);

        // Place Player 2's queen bee (not surrounded)
        auto queenBee2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr queenBee2Shared = std::move(queenBee2);
        opponent->addPiece(queenBee2Shared);
        board.addPiece(Hex(0, 0, 0), queenBee2Shared);

        const std::vector<const Player *> players = {player.get(), opponent.get()};

        const auto result = GameRules::getVictoryCondition(board, players);

        EXPECT_EQ(result, nullptr) << "The game should continue if one or both queen bees are not placed.";
    }

    /**
     * @test VictoryCondition_GameContinues
     * @brief Ensures that the game continues if both queen bees are not surrounded.
     */
    TEST_F(GameRulesTest, VictoryCondition_GameContinues) {
        // Place Player 1's queen bee
        auto queenBee1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr queenBee1Shared = std::move(queenBee1);
        player->addPiece(queenBee1Shared);
        board.addPiece(Hex(0, 0, 0), queenBee1Shared);

        // Place Player 2's queen bee
        auto queenBee2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr queenBee2Shared = std::move(queenBee2);
        opponent->addPiece(queenBee2Shared);
        board.addPiece(Hex(1, -1, 0), queenBee2Shared);

        const std::vector<const Player *> players = {player.get(), opponent.get()};

        const auto result = GameRules::getVictoryCondition(board, players);

        EXPECT_EQ(result, nullptr) << "The game should continue if one or both queen bees are not placed.";
    }
}

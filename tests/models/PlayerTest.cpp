#include <gtest/gtest.h>
#include "models/Player.h"
#include "models/Board.h"
#include "models/PieceFactory.h"
#include <memory>
#include <sstream>

namespace hive::models {
    /**
     * @class PlayerTest
     * @brief Test fixture class for Player unit tests.
     *
     * Provides a common setup for all Player-related tests, including player creation and piece management.
     */
    class PlayerTest : public testing::Test {
    protected:
        std::shared_ptr<Player> player1; /**< Player 1 used in tests */
        std::shared_ptr<Player> player2; /**< Player 2 used in tests */

        // Setup method for initializing the players
        void SetUp() override {
            Player::resetPlayerNextId();
            player1 = std::make_shared<Player>("Player1");
            player2 = std::make_shared<Player>("Player2");
        }

        // TearDown method for cleaning up
        void TearDown() override {
            // No need to manually delete player, shared_ptr takes care of it
        }
    };

    /***************************************************************************
     * Constructor Tests
     **************************************************************************/

    /**
     * @test Tests the creation of a player with a specific ID and a specific name.
     *
     * This test verifies that the player is correctly initialized with the provided ID and name.
     */
    TEST_F(PlayerTest, PlayerCreationWithSpecificIdAndName) {
        const Player player(10, "Alice");

        EXPECT_EQ(player.getId(), 10) << "Player ID should be 10.";
        EXPECT_EQ(player.getName(), "Alice") << "Player name should be Alice.";
    }

    /**
     * @test Tests the creation of a player with a specific ID and an empty name.
     *
     * This test verifies that if the name is provided as an empty string, the name remains empty.
     */
    TEST_F(PlayerTest, PlayerCreationWithSpecificIdAndEmptyName) {
        const Player player(10, "");

        EXPECT_EQ(player.getId(), 10) << "Player ID should be 10.";
        EXPECT_EQ(player.getName(), "") << "Player name should be empty.";
    }

    /**
     * @test Tests the creation of a player with the default ID and a specific name.
     *
     * This test verifies that when the ID is default, the next ID should be assigned, and the player name
     * is set as expected.
     */
    TEST_F(PlayerTest, PlayerCreationWithDefaultIdAndSpecificName) {
        const Player player("Bob");

        EXPECT_EQ(player.getId(), 3) << "Player ID should be 1 as it is the first player.";
        EXPECT_EQ(player.getName(), "Bob") << "Player name should be Bob.";
    }

    /**
     * @test Tests the creation of a player with the default ID and an empty name.
     *
     * This test verifies that when the ID is default and the name is empty, the name remains empty.
     */
    TEST_F(PlayerTest, PlayerCreationWithDefaultIdAndEmptyName) {
        const Player player{};

        EXPECT_EQ(player.getId(), 3) << "Player ID should be 1 as it is the first player.";
        EXPECT_EQ(player.getName(), "") << "Player name should be empty.";
    }

    /**
     * @test Ensures that playerNextId is updated correctly when the player ID is greater than or equal to playerNextId.
     *
     * This test verifies that when a player is created with an ID greater than the current value of playerNextId,
     * the static counter playerNextId is updated accordingly.
     */
    TEST_F(PlayerTest, PlayerCreationWithIdGreaterThanNextId) {
        const Player player(100, "Charlie");

        EXPECT_EQ(player.getId(), 100) << "Player ID should be 100.";
        EXPECT_EQ(player.getName(), "Charlie") << "Player name should be Charlie.";

        // Ensure that the static counter `playerNextId` is updated
        const Player anotherPlayer("David");
        EXPECT_EQ(anotherPlayer.getId(), 101) << "Next player ID should be 101.";
    }

    /**
     * @test Tests that playerNextId is correctly incremented when multiple players are created without specifying an ID.
     *
     * This test verifies that each new player gets a unique ID starting from 1, and the playerNextId is
     * incremented correctly.
     */
    TEST_F(PlayerTest, MultiplePlayerCreationWithDefaultIds) {
        const Player player1("Alice");
        const Player player2("Bob");

        EXPECT_EQ(player1.getId(), 3) << "First player's ID should be 1.";
        EXPECT_EQ(player2.getId(), 4) << "Second player's ID should be 2.";
    }

    /***************************************************************************
     * addPiece Tests
     **************************************************************************/

    /**
     * @test Tests adding a single piece to the player's collection.
     *
     * This test verifies that a piece is successfully added to the player's collection
     * and that the collection size increases accordingly.
     */
    TEST_F(PlayerTest, AddSinglePiece) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);

        player1->addPiece(sharedPiece);

        EXPECT_EQ(player1->getPieces().size(), 1) << "Player's piece collection should contain 1 piece.";
    }

    /**
     * @test Tests adding multiple pieces to the player's collection.
     *
     * This test ensures that multiple pieces can be added to the player's collection
     * and that the collection grows accordingly.
     */
    TEST_F(PlayerTest, AddMultiplePieces) {
        auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);

        player1->addPiece(sharedPiece1);
        player1->addPiece(sharedPiece2);

        EXPECT_EQ(player1->getPieces().size(), 2) << "Player's piece collection should contain 2 pieces.";
    }

    /**
     * @test Verifies ownership of a piece when it is added to the player's collection.
     *
     * This test checks that after adding a piece to the player's collection, the piece's
     * owner is correctly set to the player.
     */
    TEST_F(PlayerTest, AddPieceOwnership) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);

        const std::shared_ptr sharedPiece = std::move(piece);

        player1->addPiece(sharedPiece);

        EXPECT_TRUE(sharedPiece->hasOwner()) << "Piece should have an owner.";
        EXPECT_EQ(&sharedPiece->getOwner(), player1.get()) << "The owner of the added piece should be the player.";
    }

    /**
     * @test Verifies that adding a piece increases the size of the player's collection.
     *
     * This test ensures that after adding a piece, the size of the player's collection
     * increases by 1.
     */
    TEST_F(PlayerTest, AddPieceSizeIncrease) {
        auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::BEETLE);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);

        // Initially, the player has no pieces
        EXPECT_EQ(player1->getPieces().size(), 0) << "Initial piece collection size should be 0.";

        player1->addPiece(sharedPiece1);
        EXPECT_EQ(player1->getPieces().size(), 1) << "After adding 1 piece, the collection size should be 1.";

        player1->addPiece(sharedPiece2);
        EXPECT_EQ(player1->getPieces().size(), 2) << "After adding 2 pieces, the collection size should be 2.";
    }

    /**
     * @test Tests adding pieces to different players and ensuring their collections remain separate.
     *
     * This test checks that adding pieces to different players does not interfere with each other's collections.
     */
    TEST_F(PlayerTest, AddPieceToDifferentPlayers) {
        auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);

        player1->addPiece(sharedPiece1);
        player2->addPiece(sharedPiece2);

        EXPECT_EQ(player1->getPieces().size(), 1) << "Player 1 should have 1 piece.";
        EXPECT_EQ(player2->getPieces().size(), 1) << "Player 2 should have 1 piece.";
    }

    /**
     * @test Tests that adding a null piece throws an exception.
     *
     * This test ensures that the method handles cases where a null pointer is passed by throwing an exception.
     */
    TEST_F(PlayerTest, AddNullPieceThrowsException) {
        const std::shared_ptr<Piece> piece = nullptr;

        EXPECT_THROW(player1->addPiece(piece), std::invalid_argument);

        EXPECT_EQ(player1->getPieces().size(), 0) << "Adding a null piece should not change the player's collection.";
    }

    /***************************************************************************
     * getPieceCount Tests
     **************************************************************************/

    /**
     * @test Tests getting the total number of pieces owned by the player.
     *
     * This test verifies that the total number of pieces is correctly returned.
     */
    TEST_F(PlayerTest, GetPieceCount) {
        auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);

        player1->addPiece(sharedPiece1);
        player1->addPiece(sharedPiece2);

        // Test that the total number of pieces is correctly counted
        EXPECT_EQ(player1->getPieceCount(), 2) << "Player should have 2 pieces in total.";
    }

    /**
     * @test Tests getting the number of pieces of a specific type owned by the player.
     *
     * This test verifies that the number of pieces of a specific type is correctly counted.
     */
    TEST_F(PlayerTest, GetPieceCountByType) {
        auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto piece3 = PieceFactory::createPiece(enums::PieceType::ANT);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);
        const std::shared_ptr sharedPiece3 = std::move(piece3);

        player1->addPiece(sharedPiece1);
        player1->addPiece(sharedPiece2);
        player1->addPiece(sharedPiece3);

        // Test that the count of pieces of type ANT is correct
        EXPECT_EQ(player1->getPieceCount(enums::PieceType::ANT), 2) << "Player should have 2 ANT pieces.";

        // Test that the count of pieces of type QUEEN_BEE is correct
        EXPECT_EQ(player1->getPieceCount(enums::PieceType::QUEEN_BEE), 1) << "Player should have 1 QUEEN_BEE piece.";
    }

    /**
     * @test Tests getting the number of pieces when no type is provided.
     *
     * This test verifies that if no type is provided, the total number of pieces is counted.
     */
    TEST_F(PlayerTest, GetPieceCountWithoutType) {
        auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::BEETLE);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);

        player1->addPiece(sharedPiece1);
        player1->addPiece(sharedPiece2);

        // Test that the total number of pieces is correctly counted
        EXPECT_EQ(player1->getPieceCount(), 2) << "Player should have 2 pieces in total.";
    }

    /**
     * @test Tests that getting the count of a type that doesn't exist returns 0.
     *
     * This test verifies that if a player doesn't have pieces of a specific type, the count should return 0.
     */
    TEST_F(PlayerTest, GetPieceCountForNonExistentType) {
        auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT);

        const std::shared_ptr sharedPiece1 = std::move(piece1);

        player1->addPiece(sharedPiece1);

        // Test that no pieces of type QUEEN_BEE should be found
        EXPECT_EQ(player1->getPieceCount(enums::PieceType::QUEEN_BEE), 0) << "Player should have 0 QUEEN_BEE pieces.";
    }

    /***************************************************************************
     * getPieces() Tests
     **************************************************************************/

    /**
     * @test Tests that `getPieces()` returns all pieces owned by the player.
     *
     * This test ensures that all pieces are returned when no type filter is applied.
     */
    TEST_F(PlayerTest, GetPieces) {
        auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);

        player1->addPiece(sharedPiece1);
        player1->addPiece(sharedPiece2);

        const auto pieces = player1->getPieces();

        EXPECT_EQ(pieces.size(), 2) << "Player should have 2 pieces.";
    }

    /**
     * @test Tests getting pieces of a specific type using `getPieces(PieceType)`.
     *
     * This test verifies that only pieces of the specified type are returned.
     */
    TEST_F(PlayerTest, GetPiecesByType) {
        auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto piece3 = PieceFactory::createPiece(enums::PieceType::ANT);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);
        const std::shared_ptr sharedPiece3 = std::move(piece3);

        player1->addPiece(sharedPiece1);
        player1->addPiece(sharedPiece2);
        player1->addPiece(sharedPiece3);

        // Get only ANT pieces
        const auto antPieces = player1->getPieces(enums::PieceType::ANT);
        EXPECT_EQ(antPieces.size(), 2) << "Player should have 2 ANT pieces.";
        EXPECT_EQ(antPieces[0]->getType(), enums::PieceType::ANT) << "The first returned piece should be ANT.";
        EXPECT_EQ(antPieces[1]->getType(), enums::PieceType::ANT) << "The second returned piece should also be ANT.";

        // Get only QUEEN_BEE pieces
        const auto queenBeePieces = player1->getPieces(enums::PieceType::QUEEN_BEE);
        EXPECT_EQ(queenBeePieces.size(), 1) << "Player should have 1 QUEEN_BEE piece.";
        EXPECT_EQ(queenBeePieces[0]->getType(),
                  enums::PieceType::QUEEN_BEE) << "The returned piece should be QUEEN_BEE.";
    }

    /**
     * @test Tests that `getPieces(PieceType)` returns an empty vector for a type the player doesn't own.
     *
     * This test verifies that if a player doesn't own any pieces of a specified type, an empty vector is returned.
     */
    TEST_F(PlayerTest, GetPiecesByNonExistentType) {
        auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);

        player1->addPiece(sharedPiece1);
        player1->addPiece(sharedPiece2);

        // Get pieces of a type that does not exist in the collection
        const auto beetlePieces = player1->getPieces(enums::PieceType::BEETLE);
        EXPECT_EQ(beetlePieces.size(), 0) << "Player should have 0 BEETLE pieces.";
    }

    /***************************************************************************
     * getId() Tests
     **************************************************************************/

    /**
     * @test Tests that `getId()` returns the correct player ID.
     *
     * This test ensures that the `getId()` method returns the player's unique identifier.
     */
    TEST_F(PlayerTest, GetId) {
        // Create two players
        const Player playerA("PlayerA");
        const Player playerB("PlayerB");

        EXPECT_EQ(playerA.getId(), 3) << "Player A's ID should be 1.";
        EXPECT_EQ(playerB.getId(), 4) << "Player B's ID should be 2.";
    }

    /***************************************************************************
     * getName() Tests
     **************************************************************************/

    /**
     * @test Tests that `getName()` returns the correct player's name.
     *
     * This test ensures that the `getName()` method returns the player's name correctly.
     */
    TEST_F(PlayerTest, GetName) {
        const Player playerA("Alice");
        const Player playerB("Bob");

        EXPECT_EQ(playerA.getName(), "Alice") << "Player A's name should be Alice.";
        EXPECT_EQ(playerB.getName(), "Bob") << "Player B's name should be Bob.";
    }

    /***************************************************************************
     * setName() Tests
     **************************************************************************/

    /**
     * @test Tests that `setName()` correctly updates the player's name.
     *
     * This test ensures that the `setName()` method updates the player's name as expected.
     */
    TEST_F(PlayerTest, SetName) {
        Player player("Charlie");

        // Set a new name for player
        player.setName("David");

        EXPECT_EQ(player.getName(), "David") << "Player's name should be updated to David.";
    }

    /***************************************************************************
     * ownsPiece() Tests
     **************************************************************************/

    /**
     * @test Tests that `ownsPiece()` correctly identifies if a player owns a piece.
     *
     * This test ensures that the method returns true for owned pieces and false otherwise.
     */
    TEST_F(PlayerTest, OwnsPiece) {
        auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::BEETLE);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);

        player1->addPiece(sharedPiece1);

        EXPECT_TRUE(player1->ownsPiece(sharedPiece1)) << "Player should own piece1.";
        EXPECT_FALSE(player1->ownsPiece(sharedPiece2)) << "Player should not own piece2.";
    }

    /**
     * @test Tests that `ownsPiece()` returns false when given a null piece.
     *
     * This test ensures that the method handles null pointers gracefully.
     */
    TEST_F(PlayerTest, OwnsPieceNullPiece) {
        const std::shared_ptr<Piece> nullPiece = nullptr;

        EXPECT_FALSE(player1->ownsPiece(nullPiece)) << "Player should not own a null piece.";
    }

    /***************************************************************************
     * removePiece() Tests
     **************************************************************************/

    /**
     * @test Tests that `removePiece()` correctly removes a piece from the player's collection.
     *
     * This test ensures that the piece is removed and the collection size decreases.
     */
    TEST_F(PlayerTest, RemovePiece) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);

        const std::shared_ptr sharedPiece = std::move(piece);

        player1->addPiece(sharedPiece);
        EXPECT_EQ(player1->getPieces().size(), 1) << "Player should have 1 piece.";

        player1->removePiece(sharedPiece);
        EXPECT_EQ(player1->getPieces().size(), 0) << "Player should have 0 pieces after removal.";
        EXPECT_FALSE(player1->ownsPiece(sharedPiece)) << "Player should no longer own the piece.";
    }

    /**
     * @test Tests that removing a piece not owned by the player does not affect the collection.
     *
     * This test ensures that attempting to remove a non-owned piece has no effect.
     */
    TEST_F(PlayerTest, RemovePieceNotOwned) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);

        const std::shared_ptr sharedPiece = std::move(piece);

        player1->addPiece(sharedPiece);
        EXPECT_EQ(player1->getPieces().size(), 1) << "Player should have 1 piece.";

        // Create a piece not owned by player1
        auto pieceNotOwned = PieceFactory::createPiece(enums::PieceType::BEETLE);

        const std::shared_ptr pieceNotOwnedShared = std::move(pieceNotOwned);

        player1->removePiece(pieceNotOwnedShared);
        EXPECT_EQ(player1->getPieces().size(), 1) << "Player should still have 1 piece.";
    }

    /**
     * @test Tests that removing a null piece throws an exception.
     *
     * This test ensures that the method handles null pointers by throwing an exception.
     */
    TEST_F(PlayerTest, RemoveNullPieceThrowsException) {
        const std::shared_ptr<Piece> nullPiece = nullptr;

        EXPECT_THROW(player1->removePiece(nullPiece), std::invalid_argument);
    }

    /***************************************************************************
     * Equality Operator (==) Tests
     **************************************************************************/

    /**
     * @test Tests that the `==` operator correctly compares two Player objects.
     *
     * This test ensures that two players are considered equal if they have the same id.
     */
    TEST_F(PlayerTest, EqualityOperator) {
        const Player playerA(1, "Alice");
        const Player playerB(2, "Bob");

        EXPECT_TRUE(playerA == playerA) << "A player should be equal to themselves.";
        EXPECT_FALSE(playerA == playerB) << "Players with different IDs should not be equal.";

        const Player playerC(1, "Charlie");
        EXPECT_TRUE(playerA == playerC) << "Players with the same ID should be considered equal.";
    }

    /***************************************************************************
     * Inequality Operator (!=) Tests
     **************************************************************************/

    /**
     * @test Tests that the `!=` operator correctly compares two Player objects.
     *
     * This test ensures that the `!=` operator returns true if the players have different IDs.
     */
    TEST_F(PlayerTest, InequalityOperator) {
        const Player playerA(1, "Alice");
        const Player playerB(2, "Bob");

        EXPECT_FALSE(playerA != playerA) << "A player should not be different from themselves.";
        EXPECT_TRUE(playerA != playerB) << "Players with different IDs should be considered different.";

        const Player playerC(1, "Charlie");
        EXPECT_FALSE(playerA != playerC) << "Players with the same ID should not be considered different.";
    }

    /***************************************************************************
     * resetPlayerNextId() Tests
     **************************************************************************/

    /**
     * @test Tests that `resetPlayerNextId()` resets the player ID counter to 1.
     *
     * This test ensures that the static method `resetPlayerNextId()` correctly resets
     * the static player ID counter.
     */
    TEST_F(PlayerTest, ResetPlayerNextId) {
        EXPECT_EQ(player1->getId(), 1) << "Player 1's ID should be 1.";
        EXPECT_EQ(player2->getId(), 2) << "Player 2's ID should be 2.";

        // Reset the ID counter and create new players
        Player::resetPlayerNextId();
        const Player playerC("Charlie");
        const Player playerD("David");

        EXPECT_EQ(playerC.getId(), 1) << "After resetting, Player C's ID should be 1.";
        EXPECT_EQ(playerD.getId(), 2) << "After resetting, Player D's ID should be 2.";
    }

    /***************************************************************************
     * toString() Tests
     **************************************************************************/

    /**
     * @test Tests that `toString()` returns a correct string representation of the player.
     *
     * This test ensures that the `toString()` method includes the player's ID, name, and pieces.
     */
    TEST_F(PlayerTest, ToString) {
        auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::BEETLE);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);

        player1->addPiece(sharedPiece1);
        player1->addPiece(sharedPiece2);

        const std::string playerStr = player1->toString();

        EXPECT_NE(playerStr.find("Player(ID: 1, Name: Player1, Pieces: ["), std::string::npos)
            << "Player string should contain correct ID and Name.";
        EXPECT_NE(playerStr.find("Piece(ID: " + std::to_string(sharedPiece1->getId())), std::string::npos)
            << "Player string should contain piece1 ID.";
        EXPECT_NE(playerStr.find("Piece(ID: " + std::to_string(sharedPiece2->getId())), std::string::npos)
            << "Player string should contain piece2 ID.";
    }

    /***************************************************************************
     * Output Operator Tests
     **************************************************************************/

    /**
     * @test Tests that the output operator `<<` correctly outputs the player's information.
     *
     * This test ensures that streaming a player to an output stream works as expected.
     */
    TEST_F(PlayerTest, OutputOperator) {
        auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr sharedPiece = std::move(piece);
        player1->addPiece(sharedPiece);

        std::ostringstream oss;
        oss << *player1;

        const std::string output = oss.str();

        EXPECT_NE(output.find("Player(ID: 1, Name: Player1, Pieces: ["), std::string::npos)
            << "Output should contain player's ID and name.";
        EXPECT_NE(output.find("Piece(ID: " + std::to_string(sharedPiece->getId())), std::string::npos)
            << "Output should contain piece's ID.";
    }
} // namespace hive::models

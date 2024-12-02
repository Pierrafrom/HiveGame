#include <gtest/gtest.h>
#include "models/Piece.h"
#include "models/Player.h"
#include "models/enums/PieceType.h"
#include <memory>
#include "models/Board.h"
#include "models/PieceFactory.h"

namespace hive::models {
    /**
     * @brief Test fixture class for Piece unit tests.
     */
    class PieceTest : public testing::Test {
    protected:
        Board board; /**< Board instance used in tests */
        std::shared_ptr<Player> player1; /**< First player */
        std::shared_ptr<Player> player2; /**< Second player */

        void SetUp() override {
            player1 = std::make_shared<Player>("Player1");
            player2 = std::make_shared<Player>("Player2");
        }

        void TearDown() override {
            // No dynamic memory to clean up
        }
    };

    /***************************************************************************
     * Piece Creation Tests
     **************************************************************************/

    /**
     * @test Tests that a Piece is created with an auto-incremented ID.
     *
     * This test ensures that when a piece is created without specifying an ID,
     * it gets a unique ID based on the pieceNextId static counter.
     */
    TEST_F(PieceTest, CreatePieceWithAutoIncrementedId) {
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::ANT);

        EXPECT_NE(piece1->getId(), piece2->getId());
        EXPECT_GT(piece1->getId(), 0);
        EXPECT_GT(piece2->getId(), piece1->getId());
    }

    /**
     * @test Tests that a Piece is created with a specified ID.
     *
     * This test ensures that when a piece is created with a specific ID,
     * that ID is correctly assigned to the piece.
     */
    TEST_F(PieceTest, CreatePieceWithSpecifiedId) {
        constexpr size_t customId = 42;
        const auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE, customId);

        EXPECT_EQ(piece->getId(), customId);
        EXPECT_EQ(piece->getType(), enums::PieceType::BEETLE);
    }

    /**
     * @test Tests that the type of a piece is correctly assigned during creation.
     *
     * This test ensures that the correct type (e.g., QueenBee, Ant) is assigned to the piece.
     */
    TEST_F(PieceTest, PieceTypeAssignment) {
        const auto queenBee = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        const auto beetle = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const auto grasshopper = PieceFactory::createPiece(enums::PieceType::GRASSHOPPER);
        const auto spider = PieceFactory::createPiece(enums::PieceType::SPIDER);
        const auto ladybug = PieceFactory::createPiece(enums::PieceType::LADYBUG);
        const auto mosquito = PieceFactory::createPiece(enums::PieceType::MOSQUITO);
        const auto pillbug = PieceFactory::createPiece(enums::PieceType::PILLBUG);

        EXPECT_EQ(queenBee->getType(), enums::PieceType::QUEEN_BEE);
        EXPECT_EQ(ant->getType(), enums::PieceType::ANT);
        EXPECT_EQ(beetle->getType(), enums::PieceType::BEETLE);
        EXPECT_EQ(grasshopper->getType(), enums::PieceType::GRASSHOPPER);
        EXPECT_EQ(spider->getType(), enums::PieceType::SPIDER);
        EXPECT_EQ(ladybug->getType(), enums::PieceType::LADYBUG);
        EXPECT_EQ(mosquito->getType(), enums::PieceType::MOSQUITO);
        EXPECT_EQ(pillbug->getType(), enums::PieceType::PILLBUG);
    }

    /***************************************************************************
     * Piece Owner Tests
     **************************************************************************/

    /**
     * @test Tests setting and getting the owner of a piece.
     *
     * This test ensures that the owner can be correctly assigned and retrieved.
     */
    TEST_F(PieceTest, SetAndGetOwner) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        piece->setOwner(player1);

        EXPECT_TRUE(piece->hasOwner());
        EXPECT_EQ(&piece->getOwner(), player1.get());
    }

    /**
     * @test Tests getting the owner when it is not set.
     *
     * This test ensures that trying to get the owner when it is not set throws an exception.
     */
    TEST_F(PieceTest, GetOwnerWhenNotSetThrowsException) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);

        EXPECT_FALSE(piece->hasOwner());
        EXPECT_THROW(piece->getOwner(), std::runtime_error);
    }

    /***************************************************************************
     * Move Strategy Tests
     **************************************************************************/

    /**
     * @test Tests that a piece has a movement strategy assigned during creation.
     *
     * This test ensures that the movement strategy is correctly set for each piece.
     */
    TEST_F(PieceTest, PieceMoveStrategy) {
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::ANT);

        EXPECT_NO_THROW(piece1->getMoveStrategy()) << "The piece should have a move strategy assigned.";
        EXPECT_NO_THROW(piece2->getMoveStrategy()) << "The piece should have a move strategy assigned.";
    }

    /**
     * @test Tests setting a null movement strategy throws an exception.
     *
     * This test ensures that setting a null move strategy is not allowed.
     */
    TEST_F(PieceTest, SetNullMoveStrategyThrowsException) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);

        EXPECT_THROW(piece->setMoveStrategy(nullptr), std::invalid_argument);
    }

    /**
     * @test Tests getting the move strategy when it is not set.
     *
     * This test ensures that trying to get the move strategy when it is not set throws an exception.
     */
    TEST_F(PieceTest, GetMoveStrategyWhenNotSetThrowsException) {
        // Create a piece without a move strategy
        class TestPiece : public Piece {
        public:
            explicit TestPiece() : Piece(pieceNextId++, enums::PieceType::ANT) {
            }
        };

        const auto piece = std::make_unique<TestPiece>();

        EXPECT_THROW(piece->getMoveStrategy(), std::runtime_error);
    }

    /***************************************************************************
     * Position Tests
     **************************************************************************/

    /**
     * @test Tests setting and getting the position of a piece.
     *
     * This test ensures that the position can be correctly assigned and retrieved.
     */
    TEST_F(PieceTest, SetAndGetPosition) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::SPIDER);
        const Hex position(0, 0, 0);

        piece->setPosition(position);

        EXPECT_TRUE(piece->getPosition().has_value());
        EXPECT_EQ(piece->getPosition().value(), position);
    }

    /**
     * @test Tests getting the position when it is not set.
     *
     * This test ensures that the position is initially not set.
     */
    TEST_F(PieceTest, GetPositionWhenNotSet) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::LADYBUG);

        EXPECT_FALSE(piece->getPosition().has_value());
    }

    /***************************************************************************
     * Equality Operator Tests
     **************************************************************************/

    /**
     * @test Tests the equality operator (==) for comparing two pieces.
     *
     * This test ensures that two pieces are considered equal if they have the same ID, type, and owner.
     */
    TEST_F(PieceTest, EqualityOperator) {
        constexpr size_t pieceId = 42;
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT, pieceId);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::ANT, pieceId);

        piece1->setOwner(player1);
        piece2->setOwner(player1);

        EXPECT_TRUE(*piece1 == *piece2) << "Pieces with the same ID, type, and owner should be considered equal.";
    }

    /**
     * @test Tests the inequality operator (!=) for comparing two pieces.
     *
     * This test ensures that two pieces are considered different if they have different IDs.
     */
    TEST_F(PieceTest, InequalityOperator) {
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT, 42);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::ANT, 99);

        EXPECT_TRUE(*piece1 != *piece2) << "Pieces with different IDs should be considered different.";
    }

    /***************************************************************************
     * Neighboring Pieces Tests
     **************************************************************************/

    /**
     * @test Tests getting neighboring pieces of a piece.
     *
     * This test ensures that the neighboring pieces are correctly identified.
     */
    TEST_F(PieceTest, GetNeighbors) {
        // Create and place pieces on the board
        const auto queenBee = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        const auto beetle = PieceFactory::createPiece(enums::PieceType::BEETLE);

        // Set positions
        queenBee->setPosition(Hex(0, 0, 0));
        ant->setPosition(Hex(0, 1, -1));
        beetle->setPosition(Hex(0, 2, -2));

        // Add pieces to the board
        board.addPiece(queenBee->getPosition().value(), std::shared_ptr<Piece>(queenBee.get(), [](Piece *) {
        }));
        board.addPiece(ant->getPosition().value(), std::shared_ptr<Piece>(ant.get(), [](Piece *) {
        }));
        board.addPiece(beetle->getPosition().value(), std::shared_ptr<Piece>(beetle.get(), [](Piece *) {
        }));

        // Get neighbors of QueenBee
        const auto neighbors = queenBee->getNeighbors(board);

        // Verify that the neighbors are correct
        EXPECT_EQ(neighbors.size(), 1) << "The QueenBee should have 1 neighbor.";
        EXPECT_EQ(neighbors[0]->getType(), enums::PieceType::ANT);
    }

    /**
     * @test Tests that an isolated piece has no neighbors.
     *
     * This test ensures that when a piece is alone on the board, it has no neighbors.
     */
    TEST_F(PieceTest, NoNeighborsForIsolatedPiece) {
        // Place QueenBee in an isolated position on the board
        const auto queenBee = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        queenBee->setPosition(Hex(0, 0, 0));
        board.addPiece(queenBee->getPosition().value(), std::shared_ptr<Piece>(queenBee.get(), [](Piece *) {
        }));

        // Get neighbors of QueenBee
        const auto neighbors = queenBee->getNeighbors(board);

        EXPECT_TRUE(neighbors.empty()) << "The QueenBee should have no neighbors when isolated.";
    }

    /***************************************************************************
     * Edge Case Tests
     **************************************************************************/

    /**
     * @test Tests that creating a piece with an invalid type throws an exception.
     *
     * This test ensures that the PieceFactory correctly handles invalid piece types.
     */
    TEST_F(PieceTest, InvalidPieceTypeThrowsException) {
        EXPECT_THROW(PieceFactory::createPiece(static_cast<enums::PieceType>(999)), std::invalid_argument)
            << "Creating a piece with an invalid type should throw an exception.";
    }

    /**
     * @test Tests the stream insertion operator for Piece.
     *
     * This test ensures that the Piece can be outputted to an output stream.
     */
    TEST_F(PieceTest, OutputOperator) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::MOSQUITO);
        piece->setOwner(player1);
        piece->setPosition(Hex(0, 0, 0));

        std::ostringstream oss;
        oss << *piece;

        EXPECT_FALSE(oss.str().empty()) << "The output operator should produce a non-empty string.";
    }

    /**
     * @test Tests moving a piece (position change).
     *
     * This test ensures that the position of a piece can be updated.
     */
    TEST_F(PieceTest, MovePiece) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::PILLBUG);
        const Hex initialPosition(0, 0, 0);
        const Hex newPosition(1, -1, 0);

        piece->setPosition(initialPosition);
        EXPECT_EQ(piece->getPosition().value(), initialPosition);

        piece->setPosition(newPosition);
        EXPECT_EQ(piece->getPosition().value(), newPosition);
    }
} // namespace hive::models

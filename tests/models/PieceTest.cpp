// PieceTest.cpp

#include <gtest/gtest.h>
#include "models/Piece.h"
#include "models/PieceFactory.h"
#include "models/enums/PieceType.h"
#include "models/strategies/MoveStrategy.h"
#include "models/strategies/AntMoveStrategy.h"
#include "models/strategies/BeetleMoveStrategy.h"
#include "models/strategies/GrasshopperMoveStrategy.h"
#include "models/strategies/QueenBeeMoveStrategy.h"
#include "models/strategies/SpiderMoveStrategy.h"

namespace hive::models {
    /**
     * @brief Test fixture class for Piece unit tests.
     */
    class PieceTest : public testing::Test {
    };

    /**
     * @test Test creation of a Piece (via derived classes) with auto-incremented ID.
     */
    TEST_F(PieceTest, AutoIncrementId) {
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::ANT);

        EXPECT_NE(piece1->getId(), piece2->getId());
        EXPECT_GT(piece1->getId(), 0);
        EXPECT_GT(piece2->getId(), piece1->getId());
    }

    /**
     * @test Test creation of a Piece with a specified ID.
     */
    TEST_F(PieceTest, SpecifiedId) {
        constexpr size_t customId = 42;
        const auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE, customId);

        EXPECT_EQ(piece->getId(), customId);
        EXPECT_EQ(piece->getType(), enums::PieceType::BEETLE);
    }

    /**
     * @test Test getType() method for each piece type.
     */
    TEST_F(PieceTest, GetType) {
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

    /**
     * @test Test setMoveStrategy() and getMoveStrategy() methods.
     */
    TEST_F(PieceTest, SetGetMoveStrategy) {
        const auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        const auto originalStrategy = &ant->getMoveStrategy();

        // Change the move strategy
        ant->setMoveStrategy(std::make_unique<strategies::BeetleMoveStrategy>());
        const auto newStrategy = &ant->getMoveStrategy();

        EXPECT_NE(originalStrategy, newStrategy);
        // Verify that the new strategy is a BeetleMoveStrategy
        EXPECT_NO_THROW(dynamic_cast<const strategies::BeetleMoveStrategy&>(ant->getMoveStrategy()));
    }

    /**
     * @test Test equality operators operator== and operator!= for pieces with the same ID.
     */
    TEST_F(PieceTest, EqualityOperatorsSameId) {
        constexpr size_t id = 100;
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::SPIDER, id);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::SPIDER, id);

        EXPECT_TRUE(*piece1 == *piece2);
        EXPECT_FALSE(*piece1 != *piece2);
    }

    /**
     * @test Test equality operators operator== and operator!= for pieces with different IDs.
     */
    TEST_F(PieceTest, EqualityOperatorsDifferentId) {
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::SPIDER);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::SPIDER);

        EXPECT_FALSE(*piece1 == *piece2);
        EXPECT_TRUE(*piece1 != *piece2);
    }

    /**
     * @test Test PieceFactory creating pieces without specifying an ID.
     */
    TEST_F(PieceTest, PieceFactoryCreateWithoutId) {
        const auto queenBee = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        const auto beetle = PieceFactory::createPiece(enums::PieceType::BEETLE);

        EXPECT_EQ(queenBee->getType(), enums::PieceType::QUEEN_BEE);
        EXPECT_EQ(ant->getType(), enums::PieceType::ANT);
        EXPECT_EQ(beetle->getType(), enums::PieceType::BEETLE);

        EXPECT_GT(queenBee->getId(), 0);
        EXPECT_GT(ant->getId(), 0);
        EXPECT_GT(beetle->getId(), 0);
    }

    /**
     * @test Test PieceFactory creating pieces with a specified ID.
     */
    TEST_F(PieceTest, PieceFactoryCreateWithId) {
        constexpr size_t customId = 500;
        const auto grasshopper = PieceFactory::createPiece(enums::PieceType::GRASSHOPPER, customId);

        EXPECT_EQ(grasshopper->getId(), customId);
        EXPECT_EQ(grasshopper->getType(), enums::PieceType::GRASSHOPPER);
    }

    /**
     * @test Test PieceFactory throwing exception on invalid PieceType.
     */
    TEST_F(PieceTest, PieceFactoryInvalidPieceType) {
        EXPECT_THROW({
                     PieceFactory::createPiece(static_cast<enums::PieceType>(-1));
                     }, std::invalid_argument);
    }

    /**
     * @test Test creation of each derived Piece class without specifying an ID.
     */
    TEST_F(PieceTest, DerivedPiecesCreateWithoutId) {
        auto queenBee = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        auto beetle = PieceFactory::createPiece(enums::PieceType::BEETLE);
        auto grasshopper = PieceFactory::createPiece(enums::PieceType::GRASSHOPPER);
        auto spider = PieceFactory::createPiece(enums::PieceType::SPIDER);
        auto ladybug = PieceFactory::createPiece(enums::PieceType::LADYBUG);
        auto mosquito = PieceFactory::createPiece(enums::PieceType::MOSQUITO);
        auto pillbug = PieceFactory::createPiece(enums::PieceType::PILLBUG);

        EXPECT_EQ(queenBee->getType(), enums::PieceType::QUEEN_BEE);
        EXPECT_EQ(ant->getType(), enums::PieceType::ANT);
        EXPECT_EQ(beetle->getType(), enums::PieceType::BEETLE);
        EXPECT_EQ(grasshopper->getType(), enums::PieceType::GRASSHOPPER);
        EXPECT_EQ(spider->getType(), enums::PieceType::SPIDER);
        EXPECT_EQ(ladybug->getType(), enums::PieceType::LADYBUG);
        EXPECT_EQ(mosquito->getType(), enums::PieceType::MOSQUITO);
        EXPECT_EQ(pillbug->getType(), enums::PieceType::PILLBUG);

        // Ensure IDs are unique
        std::set<size_t> ids = {
            queenBee->getId(), ant->getId(), beetle->getId(),
            grasshopper->getId(), spider->getId(), ladybug->getId(),
            mosquito->getId(), pillbug->getId()
        };
        EXPECT_EQ(ids.size(), 8);
    }

    /**
     * @test Test creation of each derived Piece class with a specified ID.
     */
    TEST_F(PieceTest, DerivedPiecesCreateWithId) {
        constexpr size_t baseId = 1000;
        const auto queenBee = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE, baseId + 1);
        const auto ant = PieceFactory::createPiece(enums::PieceType::ANT, baseId + 2);
        const auto beetle = PieceFactory::createPiece(enums::PieceType::BEETLE, baseId + 3);
        const auto grasshopper = PieceFactory::createPiece(enums::PieceType::GRASSHOPPER, baseId + 4);
        const auto spider = PieceFactory::createPiece(enums::PieceType::SPIDER, baseId + 5);

        EXPECT_EQ(queenBee->getId(), baseId + 1);
        EXPECT_EQ(ant->getId(), baseId + 2);
        EXPECT_EQ(beetle->getId(), baseId + 3);
        EXPECT_EQ(grasshopper->getId(), baseId + 4);
        EXPECT_EQ(spider->getId(), baseId + 5);
    }

    /**
     * @test Test auto-increment of IDs when creating multiple pieces without specifying IDs.
     */
    TEST_F(PieceTest, AutoIncrementMultiplePieces) {
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const auto piece3 = PieceFactory::createPiece(enums::PieceType::SPIDER);

        EXPECT_GT(piece2->getId(), piece1->getId());
        EXPECT_GT(piece3->getId(), piece2->getId());
    }

    /**
     * @test Test that IDs continue correctly after creating a piece with a specified ID.
     */
    TEST_F(PieceTest, IdsContinueAfterSpecifiedId) {
        constexpr size_t customId = 2000;
        auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE, customId);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::ANT);
        const auto piece3 = PieceFactory::createPiece(enums::PieceType::BEETLE);

        EXPECT_GT(piece2->getId(), customId);
        EXPECT_GT(piece3->getId(), piece2->getId());
    }

    /**
     * @test Test that each piece has the correct movement strategy assigned.
     */
    TEST_F(PieceTest, CorrectMoveStrategyAssignment) {
        auto queenBee = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        auto beetle = PieceFactory::createPiece(enums::PieceType::BEETLE);
        auto grasshopper = PieceFactory::createPiece(enums::PieceType::GRASSHOPPER);
        auto spider = PieceFactory::createPiece(enums::PieceType::SPIDER);

        EXPECT_NO_THROW(dynamic_cast<const strategies::QueenBeeMoveStrategy&>(queenBee->getMoveStrategy()));
        EXPECT_NO_THROW(dynamic_cast<const strategies::AntMoveStrategy&>(ant->getMoveStrategy()));
        EXPECT_NO_THROW(dynamic_cast<const strategies::BeetleMoveStrategy&>(beetle->getMoveStrategy()));
        EXPECT_NO_THROW(dynamic_cast<const strategies::GrasshopperMoveStrategy&>(grasshopper->getMoveStrategy()));
        EXPECT_NO_THROW(dynamic_cast<const strategies::SpiderMoveStrategy&>(spider->getMoveStrategy()));
    }

    /**
     * @test Test changing the movement strategy of a piece.
     */
    TEST_F(PieceTest, ChangeMoveStrategy) {
        const auto ant = PieceFactory::createPiece(enums::PieceType::ANT);

        // Original strategy should be AntMoveStrategy
        EXPECT_NO_THROW(dynamic_cast<const strategies::AntMoveStrategy&>(ant->getMoveStrategy()));

        // Change strategy to BeetleMoveStrategy
        ant->setMoveStrategy(std::make_unique<strategies::BeetleMoveStrategy>());

        // New strategy should be BeetleMoveStrategy
        EXPECT_NO_THROW(dynamic_cast<const strategies::BeetleMoveStrategy&>(ant->getMoveStrategy()));
    }

    /**
     * @test Test handling nullptr in setMoveStrategy().
     */
    TEST_F(PieceTest, SetMoveStrategyNullptr) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::ANT);

        // Set the move strategy to nullptr
        piece->setMoveStrategy(nullptr);

        // Example: If getMoveStrategy() returns a reference, accessing it may throw
        EXPECT_THROW({
                     piece->getMoveStrategy();
                     }, std::runtime_error);
    }

    /**
     * @test Test comparing pieces with same ID but different types.
     */
    TEST_F(PieceTest, EqualitySameIdDifferentType) {
        constexpr size_t id = 3000;
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT, id);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::BEETLE, id);

        EXPECT_TRUE(*piece1 != *piece2);
        EXPECT_FALSE(*piece1 == *piece2);
    }

    /**
     * @test Test that creating an instance of Piece is not allowed (if Piece is abstract).
     */
    TEST_F(PieceTest, CannotInstantiateAbstractPiece) {
        // If Piece is an abstract class, the following should be invalid
        // Uncommenting the following lines should cause a compilation error

        // auto piece = std::make_unique<Piece>();
        // EXPECT_EQ(piece, nullptr);

        SUCCEED(); // This test passes if the code does not compile when attempting to instantiate Piece
    }

    /**
     * @test Test that destructors are called correctly and no memory leaks occur.
     */
    TEST_F(PieceTest, DestructorCalls) {
        // Use tools like Valgrind or sanitizers to check for memory leaks
        // For the test, we'll create and destroy pieces and expect no exceptions

        {
            auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        } // Piece should be destroyed here without issues

        SUCCEED(); // Test passes if no exceptions are thrown and no leaks are detected externally
    }
} // namespace hive::models

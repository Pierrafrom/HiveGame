// MoveTest.cpp
#include <gtest/gtest.h>
#include "models/Move.h"
#include "models/Board.h"
#include "models/PieceFactory.h"
#include <memory>
#include <ranges>

namespace hive::models {
    /**
     * @class MoveTest
     * @brief Test fixture class for Move unit tests.
     *
     * Provides a common setup for all Move-related tests, including a Board and helper functions.
     */
    class MoveTest : public testing::Test {
    protected:
        Board board; /**< Game board used in tests */
        Player *player = nullptr; /**< Pointer to the player performing the move */

        // Setup method for initializing the player
        void SetUp() override {
            player = new Player(1); // Initialize player 1 with raw pointer
            board.clear(); // Clear the board before each test
        }

        // TearDown method for cleaning up
        void TearDown() override {
            delete player; // Clean up the player pointer manually
            // No need to manually clean up the board as it's managed by the Board class
        }
    };

    /***************************************************************************
     * Constructor Tests
     **************************************************************************/

    /**
     * @test Tests creating a PLACE move with valid inputs.
     */
    TEST_F(MoveTest, PlaceMoveConstructorValid) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const Hex to(0, 0, 0);
        const Move move(player, (piece.get()), to);

        EXPECT_EQ(move.getType(), Move::MoveType::PLACE) << "Expected MOVE type to be PLACE.";
        EXPECT_EQ(move.getPlayer(), player) << "Player pointer should match the player used in the move.";
        EXPECT_EQ(move.getTo(), to) << "Destination (to) coordinates should match.";
        EXPECT_NE(move.getPiece(), nullptr) << "Piece should not be null for a valid PLACE move.";
    }

    /**
     * @test Tests creating a PLACE move with nullptr piece.
     * @expected_exception std::invalid_argument
     */
    TEST_F(MoveTest, PlaceMoveConstructorNullPiece) {
        const Hex to(0, 0, 0);
        EXPECT_THROW({
                     Move move(player, nullptr, to);
                     }, std::invalid_argument) << "Expected invalid_argument exception for null piece.";
    }

    /**
     * @test Tests creating a MOVE move with valid inputs.
     */
    TEST_F(MoveTest, MoveConstructorValid) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        // Place the piece on the board
        board.addPiece(from, piece.get());

        const Move move(player, (piece.get()), from, to);

        EXPECT_EQ(move.getType(), Move::MoveType::MOVE) << "Expected MOVE type to be MOVE.";
        EXPECT_EQ(move.getPlayer(), player) << "Player pointer should match the player used in the move.";
        EXPECT_EQ(move.getFrom(), from) << "Starting position (from) coordinates should match.";
        EXPECT_EQ(move.getTo(), to) << "Destination (to) coordinates should match.";
        EXPECT_EQ(move.getPiece(), piece.get()) << "Piece should match the piece used in the move.";
    }

    /**
     * @test Tests creating a MOVE move with nullptr piece.
     * @expected_exception std::invalid_argument
     */
    TEST_F(MoveTest, MoveConstructorNullPiece) {
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);
        EXPECT_THROW({
                     Move move(player, nullptr, from, to);
                     }, std::invalid_argument) << "Expected invalid_argument exception for null piece in MOVE move.";
    }

    /***************************************************************************
     * execute Method Tests
     **************************************************************************/

    /**
     * @test Tests executing a PLACE move and verifying the board state.
     */
    TEST_F(MoveTest, ExecutePlaceMove) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const Hex to(0, 0, 0);

        const Move move(player, (piece.get()), to);
        move.execute(board);

        EXPECT_TRUE(board.isOccupied(to)) << "Expected position to be occupied after placing the piece.";
        EXPECT_EQ(board.getTopPiece(to),
                  piece.get()) << "The top piece at the destination should match the placed piece.";
        EXPECT_EQ(board.pieceCount(), 1) << "The piece count should be 1 after placing a piece.";
    }

    /**
     * @test Tests executing a MOVE move and verifying the board state.
     */
    TEST_F(MoveTest, ExecuteMoveMove) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        // Place the piece on the board
        board.addPiece(from, piece.get());

        const Move move(player, (piece.get()), from, to);
        move.execute(board);

        EXPECT_FALSE(board.isOccupied(from)) << "Expected the starting position to be empty after the move.";
        EXPECT_TRUE(board.isOccupied(to)) << "Expected the destination to be occupied after the move.";
        EXPECT_EQ(board.getTopPiece(to),
                  piece.get()) << "The top piece at the destination should match the moved piece.";
    }

    /***************************************************************************
     * undo Method Tests
     **************************************************************************/

    /**
     * @test Tests undoing a PLACE move and verifying the board state.
     */
    TEST_F(MoveTest, UndoPlaceMove) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const Hex to(0, 0, 0);

        // Create the move and execute it
        const Move move(player, (piece.get()), to);
        move.execute(board);

        // Verify the piece is on the board
        EXPECT_TRUE(board.isOccupied(to)) << "Piece should be on the board after executing a PLACE move.";

        // Undo the move
        move.undo(board);

        // Verify the piece is removed
        EXPECT_FALSE(board.isOccupied(to)) << "Piece should be removed from the board after undoing the PLACE move.";
        EXPECT_EQ(board.pieceCount(), 0) << "There should be no pieces on the board after undoing the PLACE move.";
    }

    /**
     * @test Tests undoing a MOVE move and verifying the board state.
     */
    TEST_F(MoveTest, UndoMoveMove) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        // Place the piece on the board
        board.addPiece(from, piece.get());

        // Create the move and execute it
        const Move move(player, (piece.get()), from, to);
        move.execute(board);

        // Verify the piece is at 'to'
        EXPECT_TRUE(board.isOccupied(to)) << "Piece should be at destination after MOVE.";
        EXPECT_FALSE(board.isOccupied(from)) << "Piece should no longer be at the starting position.";

        // Undo the move
        move.undo(board);

        // Verify the piece is back at 'from'
        EXPECT_TRUE(board.isOccupied(from)) << "Piece should be back at the starting position after undoing the MOVE.";
        EXPECT_FALSE(board.isOccupied(to)) << "Piece should no longer be at the destination after undoing the MOVE.";
    }

    /***************************************************************************
     * Accessor Method Tests
     **************************************************************************/

    /**
     * @test Tests accessing getFrom() for a MOVE move.
     */
    TEST_F(MoveTest, GetFromForMove) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::SPIDER);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        const Move move(player, (piece.get()), from, to);

        EXPECT_EQ(move.getFrom(), from) << "getFrom() should return the correct starting position for MOVE.";
    }

    /**
     * @test Tests accessing getFrom() for a PLACE move.
     * @expected_exception std::runtime_error
     */
    TEST_F(MoveTest, GetFromForPlace) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const Hex to(0, 0, 0);

        const Move move(player, (piece.get()), to);

        EXPECT_THROW({
                     move.getFrom();
                     }, std::runtime_error) << "getFrom() should throw an exception for PLACE moves.";
    }

    /**
     * @test Tests accessing getTo() for both MOVE and PLACE moves.
     */
    TEST_F(MoveTest, GetTo) {
        const auto piecePlace = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const Hex toPlace(0, 0, 0);
        const Move placeMove(player, (piecePlace.get()), toPlace);
        EXPECT_EQ(placeMove.getTo(), toPlace) << "getTo() should return the correct destination for PLACE.";

        const auto pieceMove = PieceFactory::createPiece(enums::PieceType::SPIDER);
        const Hex from(0, 0, 0);
        const Hex toMove(1, -1, 0);
        const Move move(player, (pieceMove.get()), from, toMove);
        EXPECT_EQ(move.getTo(), toMove) << "getTo() should return the correct destination for MOVE.";
    }

    /**
     * @test Tests accessing getPiece().
     */
    TEST_F(MoveTest, GetPiece) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        const Move move(player, (piece.get()), from, to);

        EXPECT_EQ(move.getPiece(), piece.get()) << "getPiece() should return the correct piece for the move.";
    }

    /**
     * @test Tests accessing getPlayer().
     */
    TEST_F(MoveTest, GetPlayerId) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const Hex to(0, 0, 0);

        const Move move(player, (piece.get()), to);

        EXPECT_EQ(move.getPlayer(), player) << "getPlayer() should return the correct player.";
    }

    /**
     * @test Tests accessing getType().
     */
    TEST_F(MoveTest, GetType) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::SPIDER);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);
        const Move move(player, (piece.get()), from, to);

        EXPECT_EQ(move.getType(), Move::MoveType::MOVE) << "getType() should return MOVE for a MOVE move.";

        const Move placeMove(player, (piece.get()), to);
        EXPECT_EQ(placeMove.getType(), Move::MoveType::PLACE) << "getType() should return PLACE for a PLACE move.";
    }

    /***************************************************************************
     * Integration Tests with Board
     **************************************************************************/

    /**
     * @test Tests executing and undoing a PLACE move.
     */
    TEST_F(MoveTest, ExecuteAndUndoPlaceMove) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const Hex to(0, 0, 0);

        const Move move(player, (piece.get()), to);

        // Execute the move
        move.execute(board);
        EXPECT_TRUE(board.isOccupied(to)) << "Board should be occupied after the PLACE move.";
        EXPECT_EQ(board.getTopPiece(to), piece.get()) << "The piece at the destination should be the one just placed.";

        // Undo the move
        move.undo(board);
        EXPECT_FALSE(board.isOccupied(to)) << "Board should be empty after undoing the PLACE move.";
    }

    /**
     * @test Tests executing and undoing a MOVE move.
     */
    TEST_F(MoveTest, ExecuteAndUndoMoveMove) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        // Place the piece on the board
        board.addPiece(from, piece.get());

        const Move move(player, (piece.get()), from, to);

        // Execute the move
        move.execute(board);
        EXPECT_FALSE(board.isOccupied(from)) << "The starting position should be empty after the move.";
        EXPECT_TRUE(board.isOccupied(to)) << "The destination position should be occupied after the move.";
        EXPECT_EQ(board.getTopPiece(to), piece.get()) << "The piece should be at the destination.";

        // Undo the move
        move.undo(board);
        EXPECT_TRUE(board.isOccupied(from)) << "The starting position should be occupied after undoing the move.";
        EXPECT_FALSE(board.isOccupied(to)) << "The destination position should be empty after undoing the move.";
        EXPECT_EQ(board.getTopPiece(from), piece.get()) << "The piece should be back at the starting position.";
    }

    /**
     * @test Tests executing multiple moves and undoing them in reverse order.
     */
    TEST_F(MoveTest, ChainedMoves) {
        std::vector<std::unique_ptr<Move> > moves;
        std::vector<Piece *> pieces;

        // Create and execute several moves
        for (std::int8_t i = 0; i < 5; ++i) {
            auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
            const Hex to(i, -i, 0);
            moves.emplace_back(std::make_unique<Move>(player, (piece.get()), to));
            pieces.push_back(piece.get());

            moves.back()->execute(board);
        }

        // Verify board state
        EXPECT_EQ(board.pieceCount(), 5) << "There should be 5 pieces on the board after executing the moves.";

        // Undo moves in reverse order
        for (const auto &move: std::ranges::reverse_view(moves)) {
            move->undo(board);
        }

        // Verify board is empty
        EXPECT_EQ(board.pieceCount(), 0) << "Board should be empty after undoing all moves.";
    }

    /***************************************************************************
     * Edge Cases
     **************************************************************************/

    /**
     * @test Ensures that both constructors handle nullptr piece correctly.
     * @expected_exception std::invalid_argument
     */
    TEST_F(MoveTest, NullPieceHandling) {
        const Hex to(0, 0, 0);
        EXPECT_THROW({
                     Move move(player, nullptr, to);
                     }, std::invalid_argument) << "Constructor should throw exception when given nullptr for piece.";

        const Hex from(0, 0, 0);
        EXPECT_THROW({
                     Move move(player, nullptr, from, to);
                     }, std::
                     invalid_argument) << "Constructor should throw exception when given nullptr for piece in MOVE.";
    }

    /**
     * @test Verifies behavior when invalid hex coordinates are provided.
     * @expected_exception std::invalid_argument
     */
    TEST_F(MoveTest, InvalidHexCoordinates) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const Hex invalidHex(100, -100, 0); // Assuming the board does not handle such distant hexes

        // Attempt a PLACE move with an invalid 'to' hex
        const Move placeMove(player, (piece.get()), invalidHex);

        // Verify that executing the move throws an exception
        EXPECT_THROW({
                     placeMove.execute(board);
                     }, std::invalid_argument) << "Executing move with invalid hex should throw exception.";

        // Verify that the board remains unchanged and does not have the piece at the invalid hex
        EXPECT_FALSE(board.isOccupied(invalidHex)) << "Board should not have the piece at the invalid hex.";
        EXPECT_EQ(board.pieceCount(), 0) << "Board should remain empty when an invalid hex is provided.";
    }

    /**
     * @test Attempts to execute a MOVE move where the starting hex is empty.
     * @expected_exception std::runtime_error
     */
    TEST_F(MoveTest, EmptyStartingHexForMove) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const Hex from(0, 0, 0); // Empty hex
        const Hex to(1, -1, 0);

        const Move move(player, (piece.get()), from, to);

        EXPECT_THROW({
                     move.execute(board);
                     }, std::runtime_error) << "Executing a MOVE from an empty starting hex should throw exception.";
    }

    /**
     * @test Ensures that overlapping moves are handled correctly.
     */
    TEST_F(MoveTest, OverlappingMoves) {
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const Hex hex(0, 0, 0);

        // Place the first piece
        board.addPiece(hex, piece1.get());

        // Attempt to place the second piece on the same hex
        const Move move(player, (piece2.get()), hex);
        move.execute(board);

        // Verify that both pieces are on the hex, with the new one on top
        EXPECT_EQ(board.getTopPiece(hex), piece2.get()) << "The second piece should be on top of the first piece.";
    }

    /**
     * @test Ensures that executing and undoing moves in quick succession does not corrupt board state.
     */
    TEST_F(MoveTest, BoardStateCorruption) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const Hex to(0, 0, 0);

        const Move move(player, (piece.get()), to);

        // Rapid execution and undo
        for (int i = 0; i < 10; ++i) {
            move.execute(board);
            move.undo(board);
        }

        // Board should be empty
        EXPECT_EQ(board.pieceCount(), 0) << "Board should be empty after rapid execution and undo.";
    }

    /***************************************************************************
     * Performance Tests
     **************************************************************************/

    /**
     * @test Executes and undoes a large number of moves to test performance.
     */
    TEST_F(MoveTest, StressTest) {
        // Hex uses int8_t to store coordinates, which limits the range of possible values (-128 to 127).
        // To avoid overflow or invalid coordinate calculations, we must limit the number of moves.
        // If we count the initial hex and add 125 pieces to the board in the same direction, the total
        // number of hexes involved will be 126. At this point, the generation of surrounding hexes will
        // attempt to create a hex at coordinate 127 (the maximum positive value for int8_t).
        // Since adding any further pieces would require generating coordinates beyond the int8_t range,
        // we cannot add more pieces without risking overflow or undefined behavior.
        // Therefore, we limit the number of moves to 125 to ensure the test remains valid.
        // Since we have a limited number of pieces per player this size of board is sufficient for the HiveGame.
        constexpr int numMoves = 125;
        std::vector<std::unique_ptr<Move> > moves;
        moves.reserve(numMoves);

        const Hex origin(0, 0, 0);
        board.addPiece(origin, PieceFactory::createPiece(enums::PieceType::QUEEN_BEE).get()); // Ensure a starting piece

        // Execute a large number of moves
        for (int i = 1; i <= numMoves; ++i) {
            const Hex to(0, -i, i); // Valid hex coordinates for each move

            // Use a dummy piece to create the hex
            auto dummyPiece = PieceFactory::createPiece(enums::PieceType::SPIDER);
            board.addPiece(to, dummyPiece.get()); // Temporarily add the dummy piece
            board.unstackPiece(to); // Remove the dummy piece to keep the hex empty

            // Create the actual move
            auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
            moves.emplace_back(std::make_unique<Move>(player, (piece.get()), to));
            EXPECT_NO_THROW(moves.back()->execute(board)) << "Executing move " << i <<
 " should not throw an exception.";
        }

        EXPECT_EQ(board.pieceCount(), numMoves + 1) << "There should be " << numMoves + 1 << " pieces on the board.";

        // Undo all moves
        for (const auto &move: std::ranges::reverse_view(moves)) {
            EXPECT_NO_THROW(move->undo(board)) << "Undoing move should not throw an exception.";
        }

        EXPECT_EQ(board.pieceCount(), 1) << "Only the initial piece should remain on the board.";
    }

    /**
     * @test Ensures that std::unique_ptr for piece is correctly managing memory.
     *
     * Note: This test assumes that you are using tools like Valgrind or sanitizers
     * to check for memory leaks externally.
     */
    TEST_F(MoveTest, MemoryUsage) { {
            const auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
            const Hex to(1, 0, -1);
            board.addPiece(Hex(0, 0, 0), piece.get());

            const Move move(player, (piece.get()), to);
            move.execute(board);

            // After moving the piece, ensure memory is freed after move is out of scope
            SUCCEED();
        }

        // Check for memory leaks (typically done with Valgrind or AddressSanitizer)
        // No additional checks in code, but this is a reminder to use such tools.
    }
}

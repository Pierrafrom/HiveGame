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
        size_t playerId = 1; /**< Player ID used in tests */
    };

    /***************************************************************************
     * Constructor Tests
     **************************************************************************/

    /**
     * @test Tests creating a PLACE move with valid inputs.
     */
    TEST_F(MoveTest, PlaceMoveConstructorValid) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const Hex to(0, 0, 0);
        const Move move(playerId, std::move(piece), to);

        EXPECT_EQ(move.getType(), Move::MoveType::PLACE);
        EXPECT_EQ(move.getPlayerId(), playerId);
        EXPECT_EQ(move.getTo(), to);
        EXPECT_NE(move.getPiece(), nullptr);
    }

    /**
     * @test Tests creating a PLACE move with nullptr piece.
     * @expected_exception std::invalid_argument
     */
    TEST_F(MoveTest, PlaceMoveConstructorNullPiece) {
        const Hex to(0, 0, 0);
        EXPECT_THROW({
                     Move move(playerId, nullptr, to);
                     }, std::invalid_argument);
    }

    /**
     * @test Tests creating a MOVE move with valid inputs.
     */
    TEST_F(MoveTest, MoveConstructorValid) {
        const auto piecePtr = PieceFactory::createPiece(enums::PieceType::SPIDER);
        Piece *piece = piecePtr.get();
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        // Place the piece on the board for consistency
        board.addPiece(from, piece);

        const Move move(playerId, piece, from, to);

        EXPECT_EQ(move.getType(), Move::MoveType::MOVE);
        EXPECT_EQ(move.getPlayerId(), playerId);
        EXPECT_EQ(move.getFrom(), from);
        EXPECT_EQ(move.getTo(), to);
        EXPECT_EQ(move.getPiece(), piece);
    }

    /**
     * @test Tests creating a MOVE move with nullptr piece.
     * @expected_exception std::invalid_argument
     */
    TEST_F(MoveTest, MoveConstructorNullPiece) {
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);
        EXPECT_THROW({
                     Move move(playerId, nullptr, from, to);
                     }, std::invalid_argument);
    }

    /***************************************************************************
     * execute Method Tests
     **************************************************************************/

    /**
     * @test Tests executing a PLACE move and verifying the board state.
     */
    TEST_F(MoveTest, ExecutePlaceMove) {
        auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        Piece *piecePtr = piece.get();
        const Hex to(0, 0, 0);

        const Move move(playerId, std::move(piece), to);
        move.execute(board);

        EXPECT_TRUE(board.isOccupied(to));
        EXPECT_EQ(board.getTopPiece(to), piecePtr);
        EXPECT_EQ(board.pieceCount(), 1);
    }

    /**
     * @test Tests executing a MOVE move and verifying the board state.
     */
    TEST_F(MoveTest, ExecuteMoveMove) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        Piece *piecePtr = piece.get();
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        // Place the piece on the board
        board.addPiece(from, piecePtr);

        const Move move(playerId, piecePtr, from, to);
        move.execute(board);

        EXPECT_FALSE(board.isOccupied(from));
        EXPECT_TRUE(board.isOccupied(to));
        EXPECT_EQ(board.getTopPiece(to), piecePtr);
    }

    /***************************************************************************
     * undo Method Tests
     **************************************************************************/

    /**
     * @test Tests undoing a PLACE move and verifying the board state.
     */
    TEST_F(MoveTest, UndoPlaceMove) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const Hex to(0, 0, 0);

        const Move move(playerId, std::move(piece), to);
        move.execute(board);

        // Verify the piece is on the board
        EXPECT_TRUE(board.isOccupied(to));

        move.undo(board);

        // Verify the piece is removed
        EXPECT_FALSE(board.isOccupied(to));
        EXPECT_EQ(board.pieceCount(), 0);
    }

    /**
     * @test Tests undoing a MOVE move and verifying the board state.
     */
    TEST_F(MoveTest, UndoMoveMove) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        Piece *piecePtr = piece.get();
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        // Place the piece on the board
        board.addPiece(from, piecePtr);

        const Move move(playerId, piecePtr, from, to);
        move.execute(board);

        // Verify the piece is at 'to'
        EXPECT_TRUE(board.isOccupied(to));
        EXPECT_FALSE(board.isOccupied(from));

        move.undo(board);

        // Verify the piece is back at 'from'
        EXPECT_TRUE(board.isOccupied(from));
        EXPECT_FALSE(board.isOccupied(to));
    }

    /***************************************************************************
     * Accessor Method Tests
     **************************************************************************/

    /**
     * @test Tests accessing getFrom() for a MOVE move.
     */
    TEST_F(MoveTest, GetFromForMove) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::SPIDER);
        Piece *piecePtr = piece.get();
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        const Move move(playerId, piecePtr, from, to);

        EXPECT_EQ(move.getFrom(), from);
    }

    /**
     * @test Tests accessing getFrom() for a PLACE move.
     * @expected_exception std::runtime_error
     */
    TEST_F(MoveTest, GetFromForPlace) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const Hex to(0, 0, 0);

        const Move move(playerId, std::move(piece), to);

        EXPECT_THROW({
                     move.getFrom();
                     }, std::runtime_error);
    }

    /**
     * @test Tests accessing getTo() for both MOVE and PLACE moves.
     */
    TEST_F(MoveTest, GetTo) {
        auto piecePlace = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const Hex toPlace(0, 0, 0);
        const Move placeMove(playerId, std::move(piecePlace), toPlace);

        EXPECT_EQ(placeMove.getTo(), toPlace);

        const auto pieceMove = PieceFactory::createPiece(enums::PieceType::BEETLE);
        Piece *piecePtr = pieceMove.get();
        const Hex from(1, -1, 0);
        const Hex toMove(2, -2, 0);

        const Move moveMove(playerId, piecePtr, from, toMove);

        EXPECT_EQ(moveMove.getTo(), toMove);
    }

    /**
     * @test Tests accessing getPiece().
     */
    TEST_F(MoveTest, GetPiece) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        Piece *piecePtr = piece.get();
        const Hex to(0, 0, 0);

        const Move move(playerId, std::move(piece), to);

        EXPECT_EQ(move.getPiece(), piecePtr);
    }

    /**
     * @test Tests accessing getPlayerId().
     */
    TEST_F(MoveTest, GetPlayerId) {
        auto piece = PieceFactory::createPiece(enums::PieceType::SPIDER);
        const Hex to(0, 0, 0);

        const Move move(playerId, std::move(piece), to);

        EXPECT_EQ(move.getPlayerId(), playerId);
    }

    /**
     * @test Tests accessing getType().
     */
    TEST_F(MoveTest, GetType) {
        auto piecePlace = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const Hex toPlace(0, 0, 0);
        const Move placeMove(playerId, std::move(piecePlace), toPlace);

        EXPECT_EQ(placeMove.getType(), Move::MoveType::PLACE);

        const auto pieceMove = PieceFactory::createPiece(enums::PieceType::BEETLE);
        Piece *piecePtr = pieceMove.get();
        const Hex from(1, -1, 0);
        const Hex toMove(2, -2, 0);

        const Move moveMove(playerId, piecePtr, from, toMove);

        EXPECT_EQ(moveMove.getType(), Move::MoveType::MOVE);
    }

    /***************************************************************************
     * Integration Tests with Board
     **************************************************************************/

    /**
     * @test Tests executing and undoing a PLACE move.
     */
    TEST_F(MoveTest, ExecuteAndUndoPlaceMove) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        Piece *piecePtr = piece.get();
        const Hex to(0, 0, 0);

        const Move move(playerId, std::move(piece), to);

        // Execute the move
        move.execute(board);
        EXPECT_TRUE(board.isOccupied(to));
        EXPECT_EQ(board.getTopPiece(to), piecePtr);

        // Undo the move
        move.undo(board);
        EXPECT_FALSE(board.isOccupied(to));
    }

    /**
     * @test Tests executing and undoing a MOVE move.
     */
    TEST_F(MoveTest, ExecuteAndUndoMoveMove) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        Piece *piecePtr = piece.get();
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        // Place the piece on the board
        board.addPiece(from, piecePtr);

        const Move move(playerId, piecePtr, from, to);

        // Execute the move
        move.execute(board);
        EXPECT_FALSE(board.isOccupied(from));
        EXPECT_TRUE(board.isOccupied(to));
        EXPECT_EQ(board.getTopPiece(to), piecePtr);

        // Undo the move
        move.undo(board);
        EXPECT_TRUE(board.isOccupied(from));
        EXPECT_FALSE(board.isOccupied(to));
        EXPECT_EQ(board.getTopPiece(from), piecePtr);
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
            Piece *piecePtr = piece.get();
            const Hex to(i, -i, 0);
            moves.emplace_back(std::make_unique<Move>(playerId, std::move(piece), to));
            pieces.push_back(piecePtr);

            moves.back()->execute(board);
        }

        // Verify board state
        EXPECT_EQ(board.pieceCount(), 5);

        // Undo moves in reverse order
        for (const auto &move: std::ranges::reverse_view(moves)) {
            move->undo(board);
        }

        // Verify board is empty
        EXPECT_EQ(board.pieceCount(), 0);
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
                     Move move(playerId, nullptr, to);
                     }, std::invalid_argument);

        const Hex from(0, 0, 0);
        EXPECT_THROW({
                     Move move(playerId, nullptr, from, to);
                     }, std::invalid_argument);
    }

    /**
     * @test Verifies behavior when invalid hex coordinates are provided.
     * @expected_exception std::invalid_argument
     */
    TEST_F(MoveTest, InvalidHexCoordinates) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const Hex invalidHex(100, -100, 0); // Assuming the board does not handle such distant hexes

        // Attempt a PLACE move with an invalid 'to' hex
        const Move placeMove(playerId, std::move(piece), invalidHex);

        // Verify that executing the move throws an exception
        EXPECT_THROW({
                     placeMove.execute(board);
                     }, std::invalid_argument);

        // Verify that the board remains unchanged and does not have the piece at the invalid hex
        EXPECT_FALSE(board.isOccupied(invalidHex));
        EXPECT_EQ(board.pieceCount(), 0);
    }


    /**
     * @test Attempts to execute a MOVE move where the starting hex is empty.
     * @expected_exception std::runtime_error
     */
    TEST_F(MoveTest, EmptyStartingHexForMove) {
        const auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        Piece *piecePtr = piece.get();
        const Hex from(0, 0, 0); // Empty hex
        const Hex to(1, -1, 0);

        const Move move(playerId, piecePtr, from, to);

        EXPECT_THROW({
                     move.execute(board);
                     }, std::runtime_error);
    }

    /**
     * @test Ensures that overlapping moves are handled correctly.
     */
    TEST_F(MoveTest, OverlappingMoves) {
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT);
        Piece *piecePtr1 = piece1.get();
        auto piece2 = PieceFactory::createPiece(enums::PieceType::BEETLE);
        Piece *piecePtr2 = piece2.get();
        const Hex hex(0, 0, 0);

        // Place the first piece
        board.addPiece(hex, piecePtr1);

        // Attempt to place the second piece on the same hex
        const Move move(playerId, std::move(piece2), hex);
        move.execute(board);

        // Verify that both pieces are on the hex, with the new one on top
        EXPECT_EQ(board.getTopPiece(hex), piecePtr2);
    }

    /**
     * @test Ensures that executing and undoing moves in quick succession does not corrupt board state.
     */
    TEST_F(MoveTest, BoardStateCorruption) {
        auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        Piece *piecePtr = piece.get();
        const Hex to(0, 0, 0);

        const Move move(playerId, std::move(piece), to);

        // Rapid execution and undo
        for (int i = 0; i < 10; ++i) {
            move.execute(board);
            move.undo(board);
        }

        // Board should be empty
        EXPECT_EQ(board.pieceCount(), 0);
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
            Piece *piecePtr = piece.get();
            moves.emplace_back(std::make_unique<Move>(playerId, std::move(piece), to));
            EXPECT_NO_THROW(moves.back()->execute(board));
        }

        EXPECT_EQ(board.pieceCount(), numMoves + 1); // Includes the initial piece

        // Undo all moves
        for (const auto &move: std::ranges::reverse_view(moves)) {
            EXPECT_NO_THROW(move->undo(board));
        }

        EXPECT_EQ(board.pieceCount(), 1); // Only the initial piece remains
    }


    /**
     * @test Ensures that std::unique_ptr for piece is correctly managing memory.
     *
     * Note: This test assumes that you are using tools like Valgrind or sanitizers
     * to check for memory leaks externally.
     */
    TEST_F(MoveTest, MemoryUsage) { {
            auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
            const Hex to(0, 0, 0);
            const Move move(playerId, std::move(piece), to);
            move.execute(board);
            move.undo(board);
        } // Move and piece should be destroyed here

        SUCCEED(); // Test passes if no memory leaks are detected externally
    }
} // namespace hive::models

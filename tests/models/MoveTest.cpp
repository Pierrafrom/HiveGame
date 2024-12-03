// MoveTest.cpp
#include <gtest/gtest.h>
#include "models/Move.h"
#include "models/Board.h"
#include "models/PieceFactory.h"
#include <memory>
#include <ranges>
#include <sstream>

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
        std::shared_ptr<Player> player; /**< Shared pointer to the player performing the move */

        // Setup method for initializing the player
        void SetUp() override {
            player = std::make_shared<Player>("TestPlayer"); // Initialize player with shared_ptr
            board.clear(); // Clear the board before each test
        }

        // TearDown method for cleaning up
        void TearDown() override {
            // No need to manually clean up shared_ptr
        }
    };

    /***************************************************************************
     * Constructor Tests
     **************************************************************************/

    /**
     * @test Tests creating a PLACE move with valid inputs.
     */
    TEST_F(MoveTest, PlaceMoveConstructorValid) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);

        const Hex to(0, 0, 0);
        const Move move(player, sharedPiece, to);

        EXPECT_EQ(move.getType(), Move::MoveType::PLACE) << "Expected MOVE type to be PLACE.";
        EXPECT_EQ(move.getPlayer(), player) << "Player pointer should match the player used in the move.";
        EXPECT_EQ(move.getTo(), to) << "Destination (to) coordinates should match.";
        EXPECT_EQ(move.getPiece(), sharedPiece) << "Piece should match the piece used in the move.";
        EXPECT_FALSE(move.getFrom().has_value()) << "From position should be nullopt for PLACE move.";
    }

    /**
     * @test Tests creating a PLACE move with null player.
     * @expected_exception std::invalid_argument
     */
    TEST_F(MoveTest, PlaceMoveConstructorNullPlayer) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex to(0, 0, 0);
        EXPECT_THROW({
                     Move move(nullptr, sharedPiece, to);
                     }, std::invalid_argument) << "Expected invalid_argument exception for null player.";
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
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        // Place the piece on the board
        board.addPiece(from, sharedPiece);

        const Move move(player, sharedPiece, from, to);

        EXPECT_EQ(move.getType(), Move::MoveType::MOVE) << "Expected MOVE type to be MOVE.";
        EXPECT_EQ(move.getPlayer(), player) << "Player pointer should match the player used in the move.";
        EXPECT_EQ(move.getFrom(), from) << "Starting position (from) coordinates should match.";
        EXPECT_EQ(move.getTo(), to) << "Destination (to) coordinates should match.";
        EXPECT_EQ(move.getPiece(), sharedPiece) << "Piece should match the piece used in the move.";
    }

    /**
     * @test Tests creating a MOVE move with null player.
     * @expected_exception std::invalid_argument
     */
    TEST_F(MoveTest, MoveConstructorNullPlayer) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);
        EXPECT_THROW({
                     Move move(nullptr, sharedPiece, from, to);
                     }, std::invalid_argument) << "Expected invalid_argument exception for null player in MOVE move.";
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
        // Créer la pièce avec PieceFactory et la convertir en std::shared_ptr
        auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr sharedPiece = std::move(piece);

        // Définir la position cible
        const Hex to(0, 0, 0);

        // Créer le mouvement
        const Move move(player, sharedPiece, to);

        // Exécuter le mouvement
        move.execute(board);

        // Vérifier l'état du plateau
        EXPECT_TRUE(board.isOccupied(to)) << "Expected position to be occupied after placing the piece.";
        EXPECT_EQ(board.getTopPiece(to),
                  sharedPiece) << "The top piece at the destination should match the placed piece.";
        EXPECT_EQ(board.pieceCount(), 1) << "The piece count should be 1 after placing a piece.";
        EXPECT_EQ(sharedPiece->getPosition(), to) << "Piece's position should be updated to the target hex.";
    }

    /**
     * @test Tests executing a MOVE move and verifying the board state.
     */
    TEST_F(MoveTest, ExecuteMoveMove) {
        auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        // Place the piece on the board
        board.addPiece(from, sharedPiece);

        const Move move(player, sharedPiece, from, to);
        move.execute(board);

        EXPECT_FALSE(board.isOccupied(from)) << "Expected the starting position to be empty after the move.";
        EXPECT_TRUE(board.isOccupied(to)) << "Expected the destination to be occupied after the move.";
        EXPECT_EQ(board.getTopPiece(to),
                  sharedPiece) << "The top piece at the destination should match the moved piece.";
        EXPECT_EQ(sharedPiece->getPosition(), to) << "Piece's position should be updated to the target hex.";
    }

    /**
     * @test Tests executing a MOVE move where the piece is not at the 'from' position.
     * @expected_exception std::runtime_error
     */
    TEST_F(MoveTest, ExecuteMovePieceNotAtFromPosition) {
        auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex from(0, 0, 0); // Piece is not here
        const Hex to(1, -1, 0);

        const Move move(player, sharedPiece, from, to);
        EXPECT_THROW({
                     move.execute(board);
                     }, std::runtime_error) << "Executing MOVE when piece is not at 'from' should throw exception.";
    }

    /***************************************************************************
     * undo Method Tests
     **************************************************************************/

    /**
     * @test Tests undoing a PLACE move and verifying the board state.
     */
    TEST_F(MoveTest, UndoPlaceMove) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex to(0, 0, 0);

        // Create the move and execute it
        const Move move(player, sharedPiece, to);
        move.execute(board);

        // Verify the piece is on the board
        EXPECT_TRUE(board.isOccupied(to)) << "Piece should be on the board after executing a PLACE move.";

        // Undo the move
        move.undo(board);

        // Verify the piece is removed
        EXPECT_FALSE(board.isOccupied(to)) << "Piece should be removed from the board after undoing the PLACE move.";
        EXPECT_EQ(board.pieceCount(), 0) << "There should be no pieces on the board after undoing the PLACE move.";
        EXPECT_FALSE(sharedPiece->getPosition().has_value()) << "Piece's position should be reset after undo.";
    }

    /**
     * @test Tests undoing a MOVE move and verifying the board state.
     */
    TEST_F(MoveTest, UndoMoveMove) {
        auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        // Place the piece on the board
        board.addPiece(from, sharedPiece);

        // Create the move and execute it
        const Move move(player, sharedPiece, from, to);
        move.execute(board);

        // Verify the piece is at 'to'
        EXPECT_TRUE(board.isOccupied(to)) << "Piece should be at destination after MOVE.";
        EXPECT_FALSE(board.isOccupied(from)) << "Piece should no longer be at the starting position.";

        // Undo the move
        move.undo(board);

        // Verify the piece is back at 'from'
        EXPECT_TRUE(board.isOccupied(from)) << "Piece should be back at the starting position after undoing the MOVE.";
        EXPECT_FALSE(board.isOccupied(to)) << "Piece should no longer be at the destination after undoing the MOVE.";
        EXPECT_EQ(board.getTopPiece(from), sharedPiece) << "The piece should be back at the starting position.";
        EXPECT_EQ(sharedPiece->getPosition(), from) << "Piece's position should be reset to the original hex.";
    }

    /**
     * @test Tests undoing a MOVE move where the piece is not at the 'to' position.
     * @expected_exception std::runtime_error
     */
    TEST_F(MoveTest, UndoMovePieceNotAtToPosition) {
        auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex initialLocation(0, 0, 0);
        const Hex to(1, -1, 0);
        const Hex wrongTo(1, 0, -1);

        // Place the piece at a different location
        board.addPiece(initialLocation, sharedPiece);
        board.movePiece(initialLocation, to);

        // Create the move
        const Move move(player, sharedPiece, initialLocation, wrongTo);

        EXPECT_THROW({
                     move.undo(board);
                     }, std::runtime_error) << "Undoing MOVE when piece is not at 'to' should throw exception.";
    }

    /***************************************************************************
     * Accessor Method Tests
     **************************************************************************/

    /**
     * @test Tests accessing getFrom() for a MOVE move.
     */
    TEST_F(MoveTest, GetFromForMove) {
        auto piece = PieceFactory::createPiece(enums::PieceType::SPIDER);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        const Move move(player, sharedPiece, from, to);

        EXPECT_EQ(move.getFrom().value(), from) << "getFrom() should return the correct starting position for MOVE.";
    }

    /**
     * @test Tests accessing getFrom() for a PLACE move.
     */
    TEST_F(MoveTest, GetFromForPlace) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex to(0, 0, 0);

        const Move move(player, sharedPiece, to);

        EXPECT_FALSE(move.getFrom().has_value()) << "getFrom() should be nullopt for PLACE moves.";
    }

    /**
     * @test Tests accessing getTo() for both MOVE and PLACE moves.
     */
    TEST_F(MoveTest, GetTo) {
        auto piecePlace = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr sharedPiecePlace = std::move(piecePlace);
        const Hex toPlace(0, 0, 0);
        const Move placeMove(player, sharedPiecePlace, toPlace);
        EXPECT_EQ(placeMove.getTo(), toPlace) << "getTo() should return the correct destination for PLACE.";

        auto pieceMove = PieceFactory::createPiece(enums::PieceType::SPIDER);
        const std::shared_ptr sharedPieceMove = std::move(pieceMove);
        const Hex from(0, 0, 0);
        const Hex toMove(1, -1, 0);
        const Move move(player, sharedPieceMove, from, toMove);
        EXPECT_EQ(move.getTo(), toMove) << "getTo() should return the correct destination for MOVE.";
    }

    /**
     * @test Tests accessing getPiece().
     */
    TEST_F(MoveTest, GetPiece) {
        auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        const Move move(player, sharedPiece, from, to);

        EXPECT_EQ(move.getPiece(), sharedPiece) << "getPiece() should return the correct piece for the move.";
    }

    /**
     * @test Tests accessing getPlayer().
     */
    TEST_F(MoveTest, GetPlayer) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex to(0, 0, 0);

        const Move move(player, sharedPiece, to);

        EXPECT_EQ(move.getPlayer(), player) << "getPlayer() should return the correct player.";
    }

    /**
     * @test Tests accessing getType().
     */
    TEST_F(MoveTest, GetType) {
        auto piece = PieceFactory::createPiece(enums::PieceType::SPIDER);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);
        const Move move(player, sharedPiece, from, to);

        EXPECT_EQ(move.getType(), Move::MoveType::MOVE) << "getType() should return MOVE for a MOVE move.";

        const Move placeMove(player, sharedPiece, to);
        EXPECT_EQ(placeMove.getType(), Move::MoveType::PLACE) << "getType() should return PLACE for a PLACE move.";
    }

    /***************************************************************************
     * toString() and Output Operator Tests
     **************************************************************************/

    /**
     * @test Tests the toString() method for both MOVE and PLACE moves.
     */
    TEST_F(MoveTest, ToString) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex to(0, 0, 0);

        const Move placeMove(player, sharedPiece, to);
        std::string placeStr = placeMove.toString();
        EXPECT_NE(placeStr.find("Move(Type: PLACE"), std::string::npos) << "toString() should indicate a PLACE move.";

        const Hex from(0, 0, 0);
        const Hex toMove(1, -1, 0);
        const Move moveMove(player, sharedPiece, from, toMove);
        std::string moveStr = moveMove.toString();
        EXPECT_NE(moveStr.find("Move(Type: MOVE"), std::string::npos) << "toString() should indicate a MOVE move.";
    }

    /**
     * @test Tests the output operator << for the Move class.
     */
    TEST_F(MoveTest, OutputOperator) {
        auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const std::shared_ptr sharedPiece = std::move(piece);
        auto player = std::make_shared<Player>(1, "TestPlayer");
        const Hex to(0, 0, 0);

        const Move move(player, sharedPiece, to);

        std::ostringstream oss;
        oss << move;

        std::string output = oss.str();
        EXPECT_NE(output.find("Move(Type: PLACE"), std::string::npos) << "Output should contain 'Move(Type: PLACE'.";
        EXPECT_NE(output.find("Player: 1"), std::string::npos) << "Output should contain 'Player: 1'.";
    }

    /***************************************************************************
     * Integration Tests with Board
     **************************************************************************/

    /**
     * @test Tests executing and undoing a PLACE move.
     */
    TEST_F(MoveTest, ExecuteAndUndoPlaceMove) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex to(0, 0, 0);

        const Move move(player, sharedPiece, to);

        // Execute the move
        move.execute(board);
        EXPECT_TRUE(board.isOccupied(to)) << "Board should be occupied after the PLACE move.";
        EXPECT_EQ(board.getTopPiece(to), sharedPiece) << "The piece at the destination should be the one just placed.";

        // Undo the move
        move.undo(board);
        EXPECT_FALSE(board.isOccupied(to)) << "Board should be empty after undoing the PLACE move.";
    }

    /**
     * @test Tests executing and undoing a MOVE move.
     */
    TEST_F(MoveTest, ExecuteAndUndoMoveMove) {
        auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        // Place the piece on the board
        board.addPiece(from, sharedPiece);

        const Move move(player, sharedPiece, from, to);

        // Execute the move
        move.execute(board);
        EXPECT_FALSE(board.isOccupied(from)) << "The starting position should be empty after the move.";
        EXPECT_TRUE(board.isOccupied(to)) << "The destination position should be occupied after the move.";
        EXPECT_EQ(board.getTopPiece(to), sharedPiece) << "The piece should be at the destination.";

        // Undo the move
        move.undo(board);
        EXPECT_TRUE(board.isOccupied(from)) << "The starting position should be occupied after undoing the move.";
        EXPECT_FALSE(board.isOccupied(to)) << "The destination position should be empty after undoing the move.";
        EXPECT_EQ(board.getTopPiece(from), sharedPiece) << "The piece should be back at the starting position.";
    }

    /**
     * @test Tests executing multiple moves and undoing them in reverse order.
     */
    TEST_F(MoveTest, ChainedMoves) {
        std::vector<std::unique_ptr<Move> > moves;
        std::vector<std::shared_ptr<Piece> > pieces;

        // Create and execute several moves
        for (std::int8_t i = 0; i < 5; ++i) {
            auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
            const std::shared_ptr sharedPiece = std::move(piece);
            Hex to(i, -i, 0);
            pieces.push_back(sharedPiece);
            moves.emplace_back(std::make_unique<Move>(player, sharedPiece, to));

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
    * @test Attempts to execute a MOVE move where the destination is occupied by another piece.
    */
    TEST_F(MoveTest, MoveToOccupiedPosition) {
        // Créer deux pièces
        auto piece1 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::BEETLE);

        // Convertir en std::shared_ptr
        const std::shared_ptr<hive::models::Piece> sharedPiece1 = std::move(piece1);
        const std::shared_ptr<hive::models::Piece> sharedPiece2 = std::move(piece2);

        // Position de départ et d'arrivée
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        // Ajouter les pièces au plateau
        board.addPiece(from, sharedPiece1);
        board.addPiece(to, sharedPiece2);

        // Créer le mouvement
        const Move move(player, sharedPiece1, from, to);

        // Exécuter le mouvement
        move.execute(board);

        // Vérifier que la pièce déplacée est maintenant au sommet de la pile
        EXPECT_EQ(board.getTopPiece(to), sharedPiece1)
            << "The piece should be on top after moving to an occupied hex.";
    }

    /**
     * @test Ensures that executing a MOVE move without proper setup throws an exception.
     * @expected_exception std::runtime_error
     */
    TEST_F(MoveTest, MoveWithoutPieceOnBoard) {
        auto piece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        // Do not place the piece on the board

        const Move move(player, sharedPiece, from, to);

        EXPECT_THROW({
                     move.execute(board);
                     }, std::runtime_error) << "Executing MOVE when piece is not on the board should throw exception.";
    }

    /**
     * @test Ensures that undoing a PLACE move when the piece is not on the 'to' hex throws an exception.
     * @expected_exception std::runtime_error
     */
    TEST_F(MoveTest, UndoPlaceMovePieceNotAtToPosition) {
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);
        const Hex to(0, 0, 0);

        const Move move(player, sharedPiece, to);

        // Do not execute the move, directly undo

        EXPECT_THROW({
                     move.undo(board);
                     }, std::runtime_error) << "Undoing PLACE move when piece is not at 'to' should throw exception.";
    }

    /***************************************************************************
     * Performance Tests
     **************************************************************************/

    /**
     * @test Executes and undoes a large number of moves to test performance.
     */
    TEST_F(MoveTest, StressTest) {
        constexpr int numMoves = 50;
        std::vector<std::unique_ptr<Move> > moves;
        moves.reserve(numMoves);

        // Execute a large number of moves
        for (int i = 0; i < numMoves; ++i) {
            Hex to(i, -i, 0);

            auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
            const std::shared_ptr sharedPiece = std::move(piece);
            moves.emplace_back(std::make_unique<Move>(player, sharedPiece, to));

            EXPECT_NO_THROW(moves.back()->execute(board)) << "Executing move " << i <<
 " should not throw an exception.";
        }

        EXPECT_EQ(board.pieceCount(), numMoves) << "There should be " << numMoves << " pieces on the board.";

        // Undo all moves
        for (const auto &move: std::ranges::reverse_view(moves)) {
            EXPECT_NO_THROW(move->undo(board)) << "Undoing move should not throw an exception.";
        }

        EXPECT_EQ(board.pieceCount(), 0) << "Board should be empty after undoing all moves.";
    }

    /**
     * @test Ensures that std::shared_ptr for pieces and players are correctly managing memory.
     *
     * Note: This test assumes that you are using tools like Valgrind or sanitizers
     * to check for memory leaks externally.
     */
    TEST_F(MoveTest, MemoryUsage) { {
            auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
            const std::shared_ptr sharedPiece = std::move(piece);
            const Hex to(1, 0, -1);
            board.addPiece(Hex(0, 0, 0), sharedPiece);

            const Move move(player, sharedPiece, Hex(0, 0, 0), to);
            move.execute(board);

            // After moving the piece, ensure memory is managed properly
            SUCCEED();
        }

        // Check for memory leaks (typically done with Valgrind or AddressSanitizer)
        // No additional checks in code, but this is a reminder to use such tools.
    }
} // namespace hive::models

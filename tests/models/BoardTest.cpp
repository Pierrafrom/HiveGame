#include <gtest/gtest.h>
#include <models/enums/Direction.h>

#include "models/Board.h"
#include "models/Hex.h"
#include "models/Piece.h"

using namespace hive::models;

// Test the singleton pattern
TEST(BoardTest, Singleton) {
    Board &board1 = Board::getInstance();
    Board &board2 = Board::getInstance();
    ASSERT_EQ(&board1, &board2) << "Board should be a singleton";
}

// Test adding a piece to an empty location
TEST(BoardTest, AddPieceToEmptyLocation) {
    Board &board = Board::getInstance();
    Piece piece(1); // Using ID 1 for the piece
    const Hex hex(0, 0, 0);
    board.addPiece(hex, &piece);
    ASSERT_EQ(board.getTopPiece(hex)->getId(), piece.getId()) << "Piece should be added to the empty location";
}

// Test adding a piece to a location with existing pieces
TEST(BoardTest, AddPieceToExistingStack) {
    Board &board = Board::getInstance();
    Piece piece1(1); // Using ID 1 for the first piece
    Piece piece2(2); // Using ID 2 for the second piece
    const Hex hex(0, 0, 0);
    board.addPiece(hex, &piece1);
    board.addPiece(hex, &piece2);
    ASSERT_EQ(board.getTopPiece(hex)->getId(), piece2.getId()) << "New piece should be on top of the stack";
}


// Test adding a piece and generating neighboring hexes
TEST(BoardTest, AddPieceAndGenerateNeighbors) {
    // Get the singleton instance of Board and clear it for testing
    Board &board = Board::getInstance();
    board.clear(); // Ensure the board is empty before the test

    Piece piece(1); // Using ID 1 for the piece
    const Hex centerHex(0, 0, 0);

    // Add a piece at the center hex
    board.addPiece(centerHex, &piece);

    // Check that the board now contains 7 hexes (1 central hex + 6 neighbors)
    ASSERT_EQ(board.getBoard().size(), 7) << "Six neighboring hexes should be generated around the center hex";

    // Iterate over each direction to compute and verify the expected neighbors
    for (const auto &direction: {
             enums::Direction::NORTH_EAST,
             enums::Direction::EAST,
             enums::Direction::SOUTH_EAST,
             enums::Direction::SOUTH_WEST,
             enums::Direction::WEST,
             enums::Direction::NORTH_WEST
         }) {
        auto [dx, dy, dz] = enums::getDirectionOffset(direction);
        Hex expectedNeighbor(centerHex.getX() + dx, centerHex.getY() + dy, centerHex.getZ() + dz);

        // Check if the expected neighboring hex is present in the board
        ASSERT_TRUE(board.getBoard().contains(expectedNeighbor))
            << "Expected neighboring hex at (" << expectedNeighbor.getX() << ", " << expectedNeighbor.getY() <<
", " << expectedNeighbor.getZ() << ") was not found in the board";
    }
}

// Test removing a piece from a location with multiple pieces
TEST(BoardTest, UnstackPieceFromMultiplePieces) {
    Board &board = Board::getInstance();
    Piece piece1(1); // Using ID 1 for the first piece
    Piece piece2(2); // Using ID 2 for the second piece
    const Hex hex(0, 0, 0);
    board.addPiece(hex, &piece1);
    board.addPiece(hex, &piece2);
    const Piece *removedPiece = board.unstackPiece(hex);
    ASSERT_EQ(removedPiece->getId(), piece2.getId()) << "Removed piece should be the top piece";
    ASSERT_EQ(board.getTopPiece(hex)->getId(), piece1.getId()) << "Top piece should now be the first piece";
}

// Test removing the only piece from a location
TEST(BoardTest, UnstackOnlyPiece) {
    Board &board = Board::getInstance();
    Piece piece(1); // Using ID 1 for the piece
    const Hex hex(0, 0, 0);
    board.addPiece(hex, &piece);
    const Piece *removedPiece = board.unstackPiece(hex);
    ASSERT_EQ(removedPiece->getId(), piece.getId()) << "Removed piece should be the only piece";
    ASSERT_THROW(board.getTopPiece(hex),
                 std::invalid_argument) << "Location should be empty after removing the only piece";
}

// Test removing the only piece and freeing unused neighbors
TEST(BoardTest, UnstackOnlyPieceAndFreeNeighbors) {
    Board &board = Board::getInstance();
    Piece piece(1); // Using ID 1 for the piece
    Piece piece2(2); // Using ID 2 for the second piece
    const Hex hex(0, 0, 0);
    const Hex hex2(1, 0, -1);
    board.addPiece(hex, &piece);
    board.addPiece(hex2, &piece2);
    // afficher tous les hexes
    for (const auto &entry: board.getBoard()) {
        const auto &hex = entry.first; // Access the Hex key in the pair
        std::cout << hex << std::endl; // Print the Hex object
    }
    board.unstackPiece(hex2);
    ASSERT_EQ(board.getBoard().size(), 7) << "Only 7 hexes should remain after freeing neighbors";
}

// Test removing a piece from a non-existent location
TEST(BoardTest, UnstackPieceFromNonExistentLocation) {
    Board &board = Board::getInstance();
    const Hex hex(100, 0, -100); // Assuming this location does not exist
    ASSERT_THROW(board.unstackPiece(hex),
                 std::invalid_argument) << "Should throw std::invalid_argument for non-existent location";
}

// Test removing a piece from an empty location
TEST(BoardTest, UnstackPieceFromEmptyLocation) {
    Board &board = Board::getInstance();
    const Hex hex(0, 0, 0);
    ASSERT_THROW(board.unstackPiece(hex),
                 std::invalid_argument) << "Should throw std::invalid_argument for empty location";
}

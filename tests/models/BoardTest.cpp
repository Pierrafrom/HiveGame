#include "models/Board.h"
#include "models/Hex.h"
#include "models/Piece.h"
#include "models/enums/Direction.h"
#include "models/PieceFactory.h"
#include <gtest/gtest.h>
#include <memory>
#include <sstream>

namespace hive::models {
    /**
     * @brief Test fixture class for Board unit tests.
     */
    class BoardTest : public testing::Test {
    protected:
        Board board; /**< Board instance used in tests */
    };

    /**
     * @test Tests adding a piece to the board and verifying it's placed correctly.
     */
    TEST_F(BoardTest, AddPiece) {
        const Hex hex(0, 0, 0);
        auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr sharedPiece = std::move(piece);

        board.addPiece(hex, sharedPiece);

        EXPECT_TRUE(board.isOccupied(hex));
        EXPECT_EQ(board.getTopPiece(hex), sharedPiece);
        EXPECT_EQ(board.pieceCount(), 1);
        EXPECT_EQ(board.getBoard().size(), 7); // Origin hex + 6 neighbors
    }

    /**
     * @test Tests unstacking a piece from the board and verifying it's removed.
     */
    TEST_F(BoardTest, UnstackPiece) {
        const Hex hex(0, 0, 0);
        auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr sharedPiece = std::move(piece);

        board.addPiece(hex, sharedPiece);

        const std::shared_ptr<Piece> removedPiece = board.unstackPiece(hex);
        EXPECT_EQ(removedPiece, sharedPiece);
        EXPECT_FALSE(board.isOccupied(hex));
        EXPECT_EQ(board.pieceCount(), 0);
        EXPECT_EQ(board.getBoard().size(), 1); // Only the origin hex remains
    }

    /**
     * @test Tests stacking multiple pieces on a single hex and verifying the top piece.
     */
    TEST_F(BoardTest, StackMultiplePieces) {
        const Hex hex(0, 0, 0);
        auto piece1 = PieceFactory::createPiece(enums::PieceType::BEETLE);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::BEETLE);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);

        board.addPiece(hex, sharedPiece1);
        board.addPiece(hex, sharedPiece2);

        EXPECT_TRUE(board.isOccupied(hex));
        EXPECT_EQ(board.getTopPiece(hex), sharedPiece2);
        EXPECT_EQ(board.pieceCount(), 2);

        // Unstack the top piece and check
        const std::shared_ptr<Piece> removedPiece = board.unstackPiece(hex);
        EXPECT_EQ(removedPiece, sharedPiece2);
        EXPECT_EQ(board.getTopPiece(hex), sharedPiece1);
        EXPECT_EQ(board.pieceCount(), 1);
    }

    /**
     * @test Tests getting neighbor hexes of a given hex.
     */
    TEST_F(BoardTest, GetNeighborHexes) {
        const Hex hex(0, 0, 0);
        std::vector<Hex> expectedNeighbors;

        for (const auto &dir: enums::getAllDirections()) {
            auto [dx, dy, dz] = getDirectionOffset(dir);
            expectedNeighbors.emplace_back(hex.getX() + dx, hex.getY() + dy, hex.getZ() + dz);
        }

        std::vector<Hex> neighbors = board.getNeighborHexes(hex);
        EXPECT_EQ(neighbors.size(), 6);

        for (const auto &neighbor: expectedNeighbors) {
            EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), neighbor), neighbors.end());
        }
    }

    /**
     * @test Tests the pieceCount method by adding multiple pieces.
     */
    TEST_F(BoardTest, PieceCount) {
        const Hex hex1(0, 0, 0);
        const Hex hex2(1, -1, 0);
        auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::SPIDER);
        auto piece3 = PieceFactory::createPiece(enums::PieceType::ANT);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);
        const std::shared_ptr sharedPiece3 = std::move(piece3);

        board.addPiece(hex1, sharedPiece1);
        board.addPiece(hex2, sharedPiece2);
        board.addPiece(hex2, sharedPiece3); // Stack on hex2

        EXPECT_EQ(board.pieceCount(), 3);
    }

    /**
     * @test Tests clearing the board.
     */
    TEST_F(BoardTest, ClearBoard) {
        const Hex hex(0, 0, 0);
        auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr sharedPiece = std::move(piece);

        board.addPiece(hex, sharedPiece);

        board.clear();
        EXPECT_EQ(board.pieceCount(), 0);
        EXPECT_FALSE(board.isOccupied(hex));
        EXPECT_EQ(board.getBoard().size(), 1); // Only the origin hex remains
    }

    /**
     * @test Tests areAllPiecesConnected when pieces are connected.
     */
    TEST_F(BoardTest, AreAllPiecesConnectedTrue) {
        // Place pieces adjacent to each other
        const Hex hex1(0, 0, 0);
        const Hex hex2(1, -1, 0);
        auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::SPIDER);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);

        board.addPiece(hex1, sharedPiece1);
        board.addPiece(hex2, sharedPiece2);

        EXPECT_TRUE(board.areAllPiecesConnected());
    }

    /**
     * @test Tests areAllPiecesConnected when pieces are not connected.
     */
    TEST_F(BoardTest, AreAllPiecesConnectedFalse) {
        // Place pieces not adjacent
        const Hex hex1(0, 0, 0);
        const Hex tmpHex(1, -1, 0); // Temporary hex to place a piece
        const Hex hex2(2, -2, 0);
        auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto tmpPiece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::SPIDER);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedTmpPiece = std::move(tmpPiece);
        const std::shared_ptr sharedPiece2 = std::move(piece2);

        board.addPiece(hex1, sharedPiece1);
        board.addPiece(tmpHex, sharedTmpPiece);
        board.addPiece(hex2, sharedPiece2);

        board.unstackPiece(tmpHex); // Remove the temporary piece

        EXPECT_FALSE(board.areAllPiecesConnected());
    }

    /**
     * @test Tests adding and removing multiple pieces and verifies dynamic creation and deletion of hexes.
     */
    TEST_F(BoardTest, DynamicHexCreationDeletion) {
        const Hex centerHex(0, 0, 0);
        const Hex adjacentHex1(1, -1, 0);
        const Hex adjacentHex2(1, 0, -1);

        auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::BEETLE);
        auto piece3 = PieceFactory::createPiece(enums::PieceType::ANT);

        std::shared_ptr sharedPiece1 = std::move(piece1);
        std::shared_ptr sharedPiece2 = std::move(piece2);
        std::shared_ptr sharedPiece3 = std::move(piece3);

        // Initially, the board should have no pieces and one hex (origin hex)
        EXPECT_EQ(board.pieceCount(), 0);
        EXPECT_EQ(board.getBoard().size(), 1);

        // Add a piece to the center and check surrounding hex creation
        board.addPiece(centerHex, sharedPiece1);

        // Verify that there is 1 piece on the board and 7 hexes (1 occupied, 6 empty neighbors)
        EXPECT_EQ(board.pieceCount(), 1);
        EXPECT_EQ(board.getBoard().size(), 7);

        // Add a piece to an adjacent hex and verify new neighbors
        board.addPiece(adjacentHex1, sharedPiece2);

        // Verify that there are 2 pieces on the board and 10 hexes (2 occupied, 8 empty neighbors)
        EXPECT_EQ(board.pieceCount(), 2);
        EXPECT_EQ(board.getBoard().size(), 10);

        // Add another piece to a different adjacent hex and verify
        board.addPiece(adjacentHex2, sharedPiece3);

        // Verify that there are 3 pieces on the board and 12 hexes (3 occupied, 9 empty neighbors)
        EXPECT_EQ(board.pieceCount(), 3);
        EXPECT_EQ(board.getBoard().size(), 12);

        // Remove the piece from the center hex and verify surrounding empty hex cleanup
        board.unstackPiece(centerHex);

        // Verify there are now 2 pieces on the board and that hexes not needed have been removed
        EXPECT_EQ(board.pieceCount(), 2);
        EXPECT_EQ(board.getBoard().size(), 10); // Some unused hexes around centerHex should have been removed

        // Remove the remaining pieces and verify that all hexes are cleared except origin
        board.unstackPiece(adjacentHex1);
        board.unstackPiece(adjacentHex2);

        // The board should now have no pieces and only the origin hex
        EXPECT_EQ(board.pieceCount(), 0);
        EXPECT_EQ(board.getBoard().size(), 1);
    }

    /**
     * @test Tests getTopPiece on an empty hex.
     */
    TEST_F(BoardTest, GetTopPieceEmptyHex) {
        const Hex hex(0, 0, 0);
        EXPECT_EQ(board.getTopPiece(hex), nullptr);
    }

    /**
     * @test Tests unstackPiece on an empty hex.
     */
    TEST_F(BoardTest, UnstackPieceEmptyHex) {
        const Hex hex(0, 0, 0);
        EXPECT_EQ(board.unstackPiece(hex), nullptr);
    }

    /**
     * @test Tests adding pieces to multiple hexes and verifying isOccupied.
     */
    TEST_F(BoardTest, IsOccupiedMultipleHexes) {
        const Hex hex1(0, 0, 0);
        const Hex hex2(1, -1, 0);
        auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::ANT);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);

        board.addPiece(hex1, sharedPiece1);
        board.addPiece(hex2, sharedPiece2);

        EXPECT_TRUE(board.isOccupied(hex1));
        EXPECT_TRUE(board.isOccupied(hex2));

        // Check a hex with no piece
        const Hex hex3(2, -2, 0);
        EXPECT_FALSE(board.isOccupied(hex3));
    }

    /**
     * @test Tests operator<< overload for Board.
     */
    TEST_F(BoardTest, OutputOperator) {
        const Hex hex(0, 0, 0);
        auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr sharedPiece = std::move(piece);
        board.addPiece(hex, sharedPiece);
        std::ostringstream oss;
        oss << board;

        // Since the output format is not specified, we cannot assert on the exact string
        // But we can ensure that it doesn't throw and outputs something
        EXPECT_FALSE(oss.str().empty());
    }

    /**
     * @test Tests that adding a piece generates surrounding hexes.
     */
    TEST_F(BoardTest, GenerateSurroundingHexes) {
        const Hex hex(0, 0, 0);
        auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr sharedPiece = std::move(piece);
        board.addPiece(hex, sharedPiece);

        const std::vector<Hex> neighbors = board.getNeighborHexes(hex);
        EXPECT_EQ(neighbors.size(), 6);

        // Add a piece to one of the neighbors
        const Hex neighborHex = neighbors[0];
        auto piece2 = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece2 = std::move(piece2);
        board.addPiece(neighborHex, sharedPiece2);

        // Now check that new neighbors are generated around the neighborHex
        const std::vector<Hex> neighborNeighbors = board.getNeighborHexes(neighborHex);
        EXPECT_EQ(neighborNeighbors.size(), 6);
    }

    /**
     * @test Tests that removing a piece frees unnecessary hexes.
     */
    TEST_F(BoardTest, FreeSurroundingHexes) {
        const Hex hex(0, 0, 0);
        auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr sharedPiece = std::move(piece);
        board.addPiece(hex, sharedPiece);

        // Remove the piece
        board.unstackPiece(hex);

        // Since we cannot access private data, we can infer behavior by checking isOccupied
        EXPECT_FALSE(board.isOccupied(hex));

        // The neighbors should not be occupied
        for (const auto &neighborHex: board.getNeighborHexes(hex)) {
            EXPECT_FALSE(board.isOccupied(neighborHex));
        }
    }

    /**
     * @test Tests pieceCount after multiple add and remove operations.
     */
    TEST_F(BoardTest, PieceCountAddRemove) {
        const Hex hex1(0, 0, 0);
        const Hex hex2(1, -1, 0);
        auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::BEETLE);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);

        board.addPiece(hex1, sharedPiece1);
        board.addPiece(hex2, sharedPiece2);

        EXPECT_EQ(board.pieceCount(), 2);

        board.unstackPiece(hex1);
        EXPECT_EQ(board.pieceCount(), 1);

        board.unstackPiece(hex2);
        EXPECT_EQ(board.pieceCount(), 0);
    }

    /**
     * @test Tests that adding a piece to an occupied hex stacks the piece.
     */
    TEST_F(BoardTest, AddPieceToOccupiedHex) {
        const Hex hex(0, 0, 0);
        auto piece1 = PieceFactory::createPiece(enums::PieceType::BEETLE);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::BEETLE);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);

        board.addPiece(hex, sharedPiece1);
        board.addPiece(hex, sharedPiece2);

        EXPECT_EQ(board.getTopPiece(hex), sharedPiece2);

        // Ensure both pieces are in the stack
        // Since we cannot access the stack directly, we can test by unstacking
        std::shared_ptr<Piece> removedPiece = board.unstackPiece(hex);
        EXPECT_EQ(removedPiece, sharedPiece2);

        removedPiece = board.unstackPiece(hex);
        EXPECT_EQ(removedPiece, sharedPiece1);

        // Now the hex should be empty
        EXPECT_FALSE(board.isOccupied(hex));
    }

    /**
     * @test Tests adding a piece to a non-existent hex (should throw an exception).
     */
    TEST_F(BoardTest, AddPieceToNonExistentHex) {
        const Hex nonExistentHex(5, -5, 0); // A hex not adjacent to any existing pieces
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);

        // Adding a piece to a non-existent hex should throw an exception
        EXPECT_THROW({
                     board.addPiece(nonExistentHex, sharedPiece);
                     }, std::invalid_argument);

        // Verify that the board remains unchanged
        EXPECT_EQ(board.pieceCount(), 0);
        EXPECT_FALSE(board.isOccupied(nonExistentHex));
    }

    /**
     * @test Tests adding a null piece to the board.
     */
    TEST_F(BoardTest, AddNullPiece) {
        const Hex hex(0, 0, 0); // An existing hex on the board

        // Attempt to add a null piece to the board, which should throw an exception
        EXPECT_THROW({
                     board.addPiece(hex, nullptr);
                     }, std::invalid_argument);

        // Verify that the board state remains unchanged
        EXPECT_FALSE(board.isOccupied(hex));
        EXPECT_EQ(board.pieceCount(), 0);
    }

    /**
     * @test Tests areAllPiecesConnected with complex configurations.
     */
    TEST_F(BoardTest, AreAllPiecesConnectedComplex) {
        // Create a connected cluster
        const Hex hex1(0, 0, 0);
        const Hex hex2(1, -1, 0);
        const Hex hex3(1, 0, -1);
        const Hex hex4(1, 1, -2);
        auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::SPIDER);
        auto piece3 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto piece4 = PieceFactory::createPiece(enums::PieceType::BEETLE);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);
        const std::shared_ptr sharedPiece3 = std::move(piece3);
        const std::shared_ptr sharedPiece4 = std::move(piece4);

        board.addPiece(hex1, sharedPiece1);
        board.addPiece(hex2, sharedPiece2);
        board.addPiece(hex3, sharedPiece3);
        board.addPiece(hex4, sharedPiece4);

        EXPECT_TRUE(board.areAllPiecesConnected());

        // Remove a piece to disconnect the cluster
        board.unstackPiece(hex3);

        EXPECT_FALSE(board.areAllPiecesConnected());
    }

    /**
     * @test Tests moving a piece from one hex to another.
     */
    TEST_F(BoardTest, MovePieceValid) {
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr sharedPiece = std::move(piece);
        board.addPiece(from, sharedPiece);

        // Ensure initial state
        EXPECT_TRUE(board.isOccupied(from));
        EXPECT_FALSE(board.isOccupied(to));

        // Move the piece
        board.movePiece(from, to);

        // Verify the piece is moved
        EXPECT_FALSE(board.isOccupied(from));
        EXPECT_TRUE(board.isOccupied(to));
        EXPECT_EQ(board.getTopPiece(to), sharedPiece);
    }

    /**
     * @test Tests moving a piece from an empty hex.
     */
    TEST_F(BoardTest, MovePieceFromEmptyHex) {
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        // Attempt to move a piece from an empty hex
        EXPECT_THROW(board.movePiece(from, to), std::runtime_error);
    }

    /**
     * @test Tests moving a piece to an occupied hex.
     */
    TEST_F(BoardTest, MovePieceToOccupiedHex) {
        const Hex from(0, 0, 0);
        const Hex to(1, -1, 0);

        auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::ANT);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);

        board.addPiece(from, sharedPiece1);
        board.addPiece(to, sharedPiece2);

        // Ensure initial state
        EXPECT_TRUE(board.isOccupied(from));
        EXPECT_TRUE(board.isOccupied(to));
        EXPECT_EQ(board.getTopPiece(from), sharedPiece1);
        EXPECT_EQ(board.getTopPiece(to), sharedPiece2);

        // Move the piece
        board.movePiece(from, to);

        // Verify the stack in the target hex
        EXPECT_FALSE(board.isOccupied(from));
        EXPECT_TRUE(board.isOccupied(to));
        EXPECT_EQ(board.getTopPiece(to), sharedPiece1); // The moved piece should be on top
    }

    /**
     * @test Tests moving a piece to a non-existent hex.
     */
    TEST_F(BoardTest, MovePieceToNonExistentHex) {
        const Hex from(0, 0, 0);
        const Hex to(5, -5, 0); // Hex not adjacent to any existing hex

        auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr sharedPiece = std::move(piece);
        board.addPiece(from, sharedPiece);

        // Attempt to move the piece to a non-existent hex
        EXPECT_THROW(board.movePiece(from, to), std::invalid_argument);
    }

    /**
     * @test Tests moving a piece that disconnects the hive.
     */
    TEST_F(BoardTest, MovePieceDisconnectsHive) {
        // Place initial pieces forming a connected hive
        const Hex hex1(0, 0, 0);
        const Hex hex2(1, -1, 0);
        const Hex hex3(2, -2, 0);

        auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto piece2 = PieceFactory::createPiece(enums::PieceType::SPIDER);
        auto piece3 = PieceFactory::createPiece(enums::PieceType::ANT);

        const std::shared_ptr sharedPiece1 = std::move(piece1);
        const std::shared_ptr sharedPiece2 = std::move(piece2);
        const std::shared_ptr sharedPiece3 = std::move(piece3);

        board.addPiece(hex1, sharedPiece1);
        board.addPiece(hex2, sharedPiece2);
        board.addPiece(hex3, sharedPiece3);

        // Move piece2 in a way that disconnects the hive
        const Hex to(3, -3, 0);
        board.movePiece(hex2, to);

        // Verify that the hive is now disconnected
        EXPECT_FALSE(board.areAllPiecesConnected());
    }

    /**
     * @test Tests unstacking a piece from a non-existent hex.
     */
    TEST_F(BoardTest, UnstackPieceNonExistentHex) {
        const Hex nonExistentHex(5, -5, 0);

        // Attempt to unstack a piece from a non-existent hex
        EXPECT_EQ(board.unstackPiece(nonExistentHex), nullptr);
    }

    /**
     * @test Tests isOccupied on a non-existent hex.
     */
    TEST_F(BoardTest, IsOccupiedNonExistentHex) {
        const Hex nonExistentHex(5, -5, 0);

        // Check if the non-existent hex is occupied
        EXPECT_FALSE(board.isOccupied(nonExistentHex));
    }

    /**
     * @test Tests retrieving a neighbor in a valid direction.
     */
    TEST_F(BoardTest, NeighborValidDirection) {
        const Hex startingHex{0, 0, 0};
        // add a piece to the board to create surrounding hexes.
        auto piece = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr sharedPiece = std::move(piece);
        board.addPiece(startingHex, sharedPiece);
        // Test neighbors in valid directions
        EXPECT_NO_THROW({
            const Hex neighbor = board.neighbor(startingHex, enums::Direction::NORTH_EAST);
            EXPECT_EQ(neighbor, Hex(1, -1, 0)) << "Neighbor in NORTH_EAST should be (1, -1, 0).";
            });

        EXPECT_NO_THROW({
            const Hex neighbor = board.neighbor(startingHex, enums::Direction::EAST);
            EXPECT_EQ(neighbor, Hex(1, 0, -1)) << "Neighbor in EAST should be (1, 0, -1).";
            });
    }

    /**
     * @test Tests retrieving a neighbor in a direction where no neighbor exists.
     */
    TEST_F(BoardTest, NeighborNonExistentDirection) {
        const Hex startingHex{0, 0, 0};

        // SOUTH_WEST has no neighboring hex defined
        EXPECT_THROW({
                     board.neighbor(startingHex, enums::Direction::SOUTH_WEST);
                     }, std::out_of_range) << "Expected exception for non-existent neighbor in SOUTH_WEST direction.";
    }

    /**
     * @test Tests retrieving a neighbor for a hex that is not on the board.
     */
    TEST_F(BoardTest, NeighborForNonExistentHex) {
        const Hex nonExistentHex{2, -2, 0};

        // Attempt to retrieve a neighbor for a hex that doesn't exist
        EXPECT_THROW({
                     board.neighbor(nonExistentHex, enums::Direction::EAST);
                     }, std::out_of_range) << "Expected exception for neighbor of non-existent hex.";
    }

    /**
     * @test Tests adding a piece to an invalid hex (coordinates do not sum to zero).
     * Note: This test cannot be executed because Hex constructor uses assert, which cannot be caught.
     */
    TEST_F(BoardTest, AddPieceToInvalidHex) {
        // Cannot create an invalid Hex because the constructor uses assert
        // If the Hex constructor is modified to throw exceptions, this test can be implemented
        SUCCEED(); // Placeholder
    }
} // namespace hive::models

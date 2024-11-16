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
        const auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        board.addPiece(hex, piece.get());

        EXPECT_TRUE(board.isOccupied(hex));
        EXPECT_EQ(board.getTopPiece(hex), piece.get());
        EXPECT_EQ(board.pieceCount(), 1);
        EXPECT_EQ(board.getBoard().size(), 7); // Origin hex + 6 neighbors
    }

    /**
     * @test Tests unstacking a piece from the board and verifying it's removed.
     */
    TEST_F(BoardTest, UnstackPiece) {
        const Hex hex(0, 0, 0);
        const auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        board.addPiece(hex, piece.get());

        Piece *removedPiece = board.unstackPiece(hex);
        EXPECT_EQ(removedPiece, piece.get());
        EXPECT_FALSE(board.isOccupied(hex));
        EXPECT_EQ(board.pieceCount(), 0);
        EXPECT_EQ(board.getBoard().size(), 1); // Only the origin hex remains
    }

    /**
     * @test Tests stacking multiple pieces on a single hex and verifying the top piece.
     */
    TEST_F(BoardTest, StackMultiplePieces) {
        const Hex hex(0, 0, 0);
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::BEETLE);

        board.addPiece(hex, piece1.get());
        board.addPiece(hex, piece2.get());

        EXPECT_TRUE(board.isOccupied(hex));
        EXPECT_EQ(board.getTopPiece(hex), piece2.get());
        EXPECT_EQ(board.pieceCount(), 2);

        // Unstack the top piece and check
        Piece *removedPiece = board.unstackPiece(hex);
        EXPECT_EQ(removedPiece, piece2.get());
        EXPECT_EQ(board.getTopPiece(hex), piece1.get());
        EXPECT_EQ(board.pieceCount(), 1);
    }

    /**
     * @test Tests getting neighbor hexes of a given hex.
     */
    TEST_F(BoardTest, GetNeighborHexes) {
        const Hex hex(0, 0, 0);
        constexpr std::array<enums::Direction, 6> directions = enums::getAllDirections();
        std::vector<Hex> expectedNeighbors;

        for (const auto &dir: directions) {
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
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::SPIDER);
        const auto piece3 = PieceFactory::createPiece(enums::PieceType::ANT);

        board.addPiece(hex1, piece1.get());
        board.addPiece(hex2, piece2.get());
        board.addPiece(hex2, piece3.get()); // Stack on hex2

        EXPECT_EQ(board.pieceCount(), 3);
    }

    /**
     * @test Tests clearing the board.
     */
    TEST_F(BoardTest, ClearBoard) {
        const Hex hex(0, 0, 0);
        const auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        board.addPiece(hex, piece.get());

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
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::SPIDER);

        board.addPiece(hex1, piece1.get());
        board.addPiece(hex2, piece2.get());

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
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const auto tmpPiece = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::SPIDER);

        board.addPiece(hex1, piece1.get());
        board.addPiece(tmpHex, tmpPiece.get());
        board.addPiece(hex2, piece2.get());

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

        // Initially, the board should have no pieces and one hex (origin hex)
        EXPECT_EQ(board.pieceCount(), 0);
        EXPECT_EQ(board.getBoard().size(), 1);

        // Add a piece to the center and check surrounding hex creation
        board.addPiece(centerHex, piece1.get());
        std::vector<Hex> neighbors1 = board.getNeighborHexes(centerHex);

        // Verify that there is 1 piece on the board and 7 hexes (1 occupied, 6 empty neighbors)
        EXPECT_EQ(board.pieceCount(), 1);
        EXPECT_EQ(board.getBoard().size(), 7);

        // Add a piece to an adjacent hex and verify new neighbors
        board.addPiece(adjacentHex1, piece2.get());
        std::vector<Hex> neighbors2 = board.getNeighborHexes(adjacentHex1);

        // Verify that there are 2 pieces on the board and 10 hexes (2 occupied, 8 empty neighbors)
        EXPECT_EQ(board.pieceCount(), 2);
        EXPECT_EQ(board.getBoard().size(), 10);

        // Add another piece to a different adjacent hex and verify
        board.addPiece(adjacentHex2, piece3.get());
        std::vector<Hex> neighbors3 = board.getNeighborHexes(adjacentHex2);

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
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::ANT);

        board.addPiece(hex1, piece1.get());
        board.addPiece(hex2, piece2.get());

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
        const auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        board.addPiece(hex, piece.get());
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
        const auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        board.addPiece(hex, piece.get());

        const std::vector<Hex> neighbors = board.getNeighborHexes(hex);
        EXPECT_EQ(neighbors.size(), 6);

        // Add a piece to one of the neighbors
        const Hex neighborHex = neighbors[0];
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::ANT);
        board.addPiece(neighborHex, piece2.get());

        // Now check that new neighbors are generated around the neighborHex
        const std::vector<Hex> neighborNeighbors = board.getNeighborHexes(neighborHex);
        EXPECT_EQ(neighborNeighbors.size(), 6);
    }

    /**
     * @test Tests that removing a piece frees unnecessary hexes.
     */
    TEST_F(BoardTest, FreeSurroundingHexes) {
        const Hex hex(0, 0, 0);
        const auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        board.addPiece(hex, piece.get());

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
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::BEETLE);

        board.addPiece(hex1, piece1.get());
        board.addPiece(hex2, piece2.get());

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
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::BEETLE);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::BEETLE);

        board.addPiece(hex, piece1.get());
        board.addPiece(hex, piece2.get());

        EXPECT_EQ(board.getTopPiece(hex), piece2.get());

        // Ensure both pieces are in the stack
        // Since we cannot access the stack directly, we can test by unstacking
        Piece *removedPiece = board.unstackPiece(hex);
        EXPECT_EQ(removedPiece, piece2.get());

        removedPiece = board.unstackPiece(hex);
        EXPECT_EQ(removedPiece, piece1.get());

        // Now the hex should be empty
        EXPECT_FALSE(board.isOccupied(hex));
    }

    /**
     * @test Tests adding a piece to a non-existent hex (should throw an exception).
     */
    TEST_F(BoardTest, AddPieceToNonExistentHex) {
        const Hex nonExistentHex(5, -5, 0); // A hex not adjacent to any existing pieces
        const auto piece = PieceFactory::createPiece(enums::PieceType::ANT);

        // Adding a piece to a non-existent hex should throw an exception
        EXPECT_THROW({
                     board.addPiece(nonExistentHex, piece.get());
                     }, std::invalid_argument);

        // Verify that the board remains unchanged
        EXPECT_EQ(board.pieceCount(), 0);
        EXPECT_FALSE(board.isOccupied(nonExistentHex));
    }

    /**
     * @test Tests adding a null piece to the board.
     * @expected_exception std::invalid_argument
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
        const auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::SPIDER);
        const auto piece3 = PieceFactory::createPiece(enums::PieceType::ANT);
        const auto piece4 = PieceFactory::createPiece(enums::PieceType::BEETLE);

        board.addPiece(hex1, piece1.get());
        board.addPiece(hex2, piece2.get());
        board.addPiece(hex3, piece3.get());
        board.addPiece(hex4, piece4.get());

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

        const auto piece = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        board.addPiece(from, piece.get());

        // Ensure initial state
        EXPECT_TRUE(board.isOccupied(from));
        EXPECT_FALSE(board.isOccupied(to));

        // Move the piece
        board.movePiece(from, to);

        // Verify the piece is moved
        EXPECT_FALSE(board.isOccupied(from));
        EXPECT_TRUE(board.isOccupied(to));
        EXPECT_EQ(board.getTopPiece(to), piece.get());
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

        const auto piece1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const auto piece2 = PieceFactory::createPiece(enums::PieceType::ANT);

        board.addPiece(from, piece1.get());
        board.addPiece(to, piece2.get());

        // Ensure initial state
        EXPECT_TRUE(board.isOccupied(from));
        EXPECT_TRUE(board.isOccupied(to));
        EXPECT_EQ(board.getTopPiece(from), piece1.get());
        EXPECT_EQ(board.getTopPiece(to), piece2.get());

        // Move the piece
        board.movePiece(from, to);

        // Verify the stack in the target hex
        EXPECT_FALSE(board.isOccupied(from));
        EXPECT_TRUE(board.isOccupied(to));
        EXPECT_EQ(board.getTopPiece(to), piece1.get()); // The moved piece should be on top
    }
} // namespace hive::models

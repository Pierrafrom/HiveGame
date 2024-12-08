#include <gtest/gtest.h>
#include <models/PieceFactory.h>
#include <models/Player.h>
#include <models/Move.h>
#include <models/GameRules.h>

#include "models/Board.h"

namespace hive::models::strategies {
    class StrategyTest : public testing::Test {
    protected:
        Board board; /**< Board instance used in tests */
        GameRules &gameRules = GameRules::getInstance();
        std::shared_ptr<Player> player1; /**< Player 1 used in tests */
        std::shared_ptr<Player> player2; /**< Player 2 used in tests */
        size_t turn = 1;

        // Pieces for player 1
        std::shared_ptr<Piece> ant1player1shared;
        std::shared_ptr<Piece> spider1player1shared;
        std::shared_ptr<Piece> beetle1player1shared;
        std::shared_ptr<Piece> grasshopper1player1shared;
        std::shared_ptr<Piece> ladybug1player1shared;
        std::shared_ptr<Piece> mosquito1player1shared;
        std::shared_ptr<Piece> pillbug1player1shared;
        std::shared_ptr<Piece> queen1player1shared;

        // Pieces for player 2
        std::shared_ptr<Piece> ant1player2shared;
        std::shared_ptr<Piece> spider1player2shared;
        std::shared_ptr<Piece> beetle1player2shared;
        std::shared_ptr<Piece> grasshopper1player2shared;
        std::shared_ptr<Piece> ladybug1player2shared;
        std::shared_ptr<Piece> mosquito1player2shared;
        std::shared_ptr<Piece> pillbug1player2shared;
        std::shared_ptr<Piece> queen1player2shared;


        // Setup method for initializing the players
        void SetUp() override {
            Player::resetPlayerNextId();
            player1 = std::make_shared<Player>("Player1");
            player2 = std::make_shared<Player>("Player2");

            const Hex origine(0,0,0);
            //create one ant piece for each player
            auto ant1player1 = PieceFactory::createPiece(enums::PieceType::ANT);
            ant1player1shared = std::move(ant1player1);
            auto ant1player2 = PieceFactory::createPiece(enums::PieceType::ANT);
            ant1player2shared = std::move(ant1player2);

            // Create spider piece for each player
            auto spider1player1 = PieceFactory::createPiece(enums::PieceType::SPIDER);
            spider1player1shared = std::move(spider1player1);
            auto spider1player2 = PieceFactory::createPiece(enums::PieceType::SPIDER);
            spider1player2shared = std::move(spider1player2);

            // Create beetle piece for each player
            auto beetle1player1 = PieceFactory::createPiece(enums::PieceType::BEETLE);
            beetle1player1shared = std::move(beetle1player1);
            auto beetle1player2 = PieceFactory::createPiece(enums::PieceType::BEETLE);
            beetle1player2shared = std::move(beetle1player2);

            // Create grasshopper piece for each player
            auto grasshopper1player1 = PieceFactory::createPiece(enums::PieceType::GRASSHOPPER);
            grasshopper1player1shared = std::move(grasshopper1player1);
            auto grasshopper1player2 = PieceFactory::createPiece(enums::PieceType::GRASSHOPPER);
            grasshopper1player2shared = std::move(grasshopper1player2);

            // Create ladybug piece for each player
            auto ladybug1player1 = PieceFactory::createPiece(enums::PieceType::LADYBUG);
            ladybug1player1shared = std::move(ladybug1player1);
            auto ladybug1player2 = PieceFactory::createPiece(enums::PieceType::LADYBUG);
            ladybug1player2shared = std::move(ladybug1player2);

            // Create mosquito piece for each player
            auto mosquito1player1 = PieceFactory::createPiece(enums::PieceType::MOSQUITO);
            mosquito1player1shared = std::move(mosquito1player1);
            auto mosquito1player2 = PieceFactory::createPiece(enums::PieceType::MOSQUITO);
            mosquito1player2shared = std::move(mosquito1player2);

            // Create pillbug piece for each player
            auto pillbug1player1 = PieceFactory::createPiece(enums::PieceType::PILLBUG);
            pillbug1player1shared = std::move(pillbug1player1);
            auto pillbug1player2 = PieceFactory::createPiece(enums::PieceType::PILLBUG);
            pillbug1player2shared = std::move(pillbug1player2);

            // Create queen piece for each player
            auto queen1player1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
            queen1player1shared = std::move(queen1player1);
            auto queen1player2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
            queen1player2shared = std::move(queen1player2);


            //==========================================================================================================

            //ant1player1 place on hex(0,0,0)
            const Move move1(player1, ant1player1shared, origine);
            move1.execute(board);
            turn++;

            std::cout << board.pieceCount() << std::endl;

            //ant1player2 place on hex(1,-1,0)
            const Move move2(player2, ant1player2shared, Hex(1,-1,0));
            move2.execute(board);
            turn++;

            //spider1player1 place on hex(-1,0,1)
            const Move move3(player1, spider1player1shared, Hex(-1,0,1));
            move3.execute(board);
            turn++;

            //spider1player2 place on hex(1,-2,1)
            const Move move4(player2, spider1player2shared, Hex(1,-2,1));
            move4.execute(board);
            turn++;

            //beetle1player1 place on hex(-1,1,0)
            const Move move5(player1, beetle1player1shared, Hex(-1,1,0));
            move5.execute(board);
            turn++;

            //beetle1player2 place on hex(2,-2,0)
            const Move move6(player2, beetle1player2shared, Hex(2,-2,0));
            move6.execute(board);
            turn++;

            //queen1player1 place on hex(0,1,-1)
            const Move move7(player1, queen1player1shared, Hex(0,1,-1));
            move7.execute(board);
            turn++;

            //queen1player2 place on hex(2,-1,-1)
            const Move move8(player2, queen1player2shared, Hex(2,-1,-1));
            move8.execute(board);
            turn++;

            //grasshopper1player1 on hex(-1,2,-1)
            const Move move9(player1, grasshopper1player1shared, Hex(-1,2,-1));
            move9.execute(board);
            turn++;

            //grasshopper1player2 on hex(3,-2,-1)
            const Move move10(player2, grasshopper1player2shared, Hex(3,-2,-1));
            move10.execute(board);
            turn++;

            //ladybug1player1 on hex(0,2,-2)
            const Move move11(player1, ladybug1player1shared, Hex(0,2,-2));
            move11.execute(board);
            turn++;

            //ladybug1player2 on hex(3,-1,-2)
            const Move move12(player2, ladybug1player2shared, Hex(3,-1,-2));
            move12.execute(board);
            turn++;

            //mosquito1player1 on hex(-1,3,-2)
            const Move move13(player1, mosquito1player1shared, Hex(-1,3,-2));
            move13.execute(board);
            turn++;

            //mosquito1player2 on hex(4,-2,-2)
            const Move move14(player2, mosquito1player2shared, Hex(4,-2,-2));
            move14.execute(board);
            turn++;

            //pillbug1player1 on hex(-2,3,-1)
            const Move move15(player1, pillbug1player1shared, Hex(-2,3,-1));
            move15.execute(board);
            turn++;

            //pillbug1player2 on hex(4,-3,-1)
            const Move move16(player2, pillbug1player2shared, Hex(4,-3,-1));
            move16.execute(board);
            turn++;



        }

        //teardown
        void TearDown() override {
            board.clear();
        }
    };

    /*******************************************************************************************************************
     * tests for QueenBeeMoveStrategy
     * ****************************************************************************************************************/

    /**
     * @test Verify that the Queen Bee can move to valid adjacent hexes.
     * @details This test checks the possible moves for the Queen Bee when it belongs to the player and
     *          is placed at position (0, 1, -1) with no blocking pieces around it.
     */
    TEST_F(StrategyTest, TestValidMoveForQueenBee) {
        // Retrieve queen1player1 already placed at (0, 1, -1)
        const std::vector<Hex> possibleMoves = queen1player1shared->getMoveStrategy().getPossibleMoves(board, *player1);

        // Check that the Queen Bee has valid adjacent moves (expected: 2 adjacent hexes are free)
        ASSERT_EQ(possibleMoves.size(), 2);
    }

    /**
     * @test Verify that no moves are available for the Queen Bee if it does not belong to the player.
     * @details This test ensures that the Queen Bee cannot move when another player owns the piece,
     *          even if it is placed on the board.
     */
    TEST_F(StrategyTest, TestNoMoveWhenPieceDoesNotBelongToPlayer) {
        // Attempt to retrieve possible moves for player2, who does not own queen1player1
        const std::vector<Hex> possibleMoves = queen1player1shared->getMoveStrategy().getPossibleMoves(board, *player2);

        // Assert that no moves are available for a player who does not own the Queen Bee
        ASSERT_EQ(possibleMoves.size(), 0);
    }

    /**
     * @test Verify that no moves are available for the Queen Bee when surrounded by blocking pieces.
     * @details This test simulates a scenario where the Queen Bee is completely surrounded by other
     *          pieces, preventing any movement.
     */
    TEST_F(StrategyTest, TestNoMovesWhenQueenBeeIsBlocked) {
        // Surround queen1player1 with blocking pieces
        auto ant2player1 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto ant3player1 = PieceFactory::createPiece(enums::PieceType::ANT);
        const std::shared_ptr<Piece> ant2player1shared = std::move(ant2player1);
        const std::shared_ptr<Piece> ant3player1shared = std::move(ant3player1);

        const Move move1(player1, ant2player1shared, Hex(1,0,-1));
        move1.execute(board);
        const Move move2(player1, ant3player1shared, Hex(1,1,-2));
        move2.execute(board);


        // Retrieve possible moves for queen1player1
        const std::vector<Hex> possibleMoves = queen1player1shared->getMoveStrategy().getPossibleMoves(board, *player1);

        // Assert that no moves are available when surrounded by blocking pieces
        ASSERT_EQ(possibleMoves.size(), 0);
    }

    /*******************************************************************************************************************
     * tests for SpiderMoveStrategy
     * ****************************************************************************************************************/

    /**
     * @test Verify that the White Spider can move to valid adjacent hexes.
     * @details This test checks the possible moves for the White Spider when it belongs to the player and
     *          is placed at position (-1, 0, 1) with no blocking pieces around it.
     */
    TEST_F(StrategyTest, TestValidMoveForWhiteSpider) {
        // Retrieve spider1player1 already placed at (-1, 0, 1)
        const std::vector<Hex> possibleMoves = spider1player1shared->getMoveStrategy().getPossibleMoves(board, *player1);

        // Check that the White Spider has valid adjacent moves (expected: 3 adjacent hexes are free based on the image)
        ASSERT_EQ(possibleMoves.size(), 2);
    }

    /**
     * @test Verify that no moves are available for the White Spider if it does not belong to the player.
     * @details This test ensures that the White Spider cannot move when another player owns the piece,
     *          even if it is placed on the board.
     */
    TEST_F(StrategyTest, TestNoMoveWhenWhiteSpiderDoesNotBelongToPlayer) {
        // Attempt to retrieve possible moves for player2, who does not own spider1player1
        const std::vector<Hex> possibleMoves = spider1player1shared->getMoveStrategy().getPossibleMoves(board, *player2);

        // Assert that no moves are available for a player who does not own the White Spider
        ASSERT_EQ(possibleMoves.size(), 0);
    }

    /**
     * @test Verify that no moves are available for the White Spider when surrounded by blocking pieces.
     * @details This test simulates a scenario where the White Spider is completely surrounded by other
     *          pieces, preventing any movement.
     */
    TEST_F(StrategyTest, TestNoMovesWhenWhiteSpiderIsBlocked) {
        // Surround spider1player1 with blocking pieces
        auto grasshopper2player1 = PieceFactory::createPiece(enums::PieceType::GRASSHOPPER);
        auto ant2player1 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto ant3player1 = PieceFactory::createPiece(enums::PieceType::ANT);
        auto beetle2player1 = PieceFactory::createPiece(enums::PieceType::BEETLE);

        const std::shared_ptr<Piece> grasshopper2player1shared = std::move(grasshopper2player1);
        const std::shared_ptr<Piece> ant2player1shared = std::move(ant2player1);
        const std::shared_ptr<Piece> ant3player1shared = std::move(ant3player1);
        const std::shared_ptr<Piece> beetle2player1shared = std::move(beetle2player1);

        const Move move2(player1, grasshopper2player1shared, Hex(-2, 1, 1));
        move2.execute(board);
        const Move move3(player1, ant2player1shared, Hex(-2, 0, 2));
        move3.execute(board);
        const Move move4(player1, ant3player1shared, Hex(-1, -1, 2));
        move4.execute(board);
        const Move move5(player1, beetle2player1shared, Hex(0, -1, 1));
        move5.execute(board);

        // Retrieve possible moves for spider1player1
        const std::vector<Hex> possibleMoves = spider1player1shared->getMoveStrategy().getPossibleMoves(board, *player1);

        // Assert that no moves are available when surrounded by blocking pieces
        ASSERT_EQ(possibleMoves.size(), 0);
    }

} // namespace hive::models::strategies

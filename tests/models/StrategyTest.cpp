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

        // Setup method for initializing the players
        void SetUp() override {
            Player::resetPlayerNextId();
            player1 = std::make_shared<Player>("Player1");
            player2 = std::make_shared<Player>("Player2");

            const Hex origine(0,0,0);
            //create one ant piece for each player
            auto ant1player1 = PieceFactory::createPiece(enums::PieceType::ANT);
            const std::shared_ptr ant1player1shared = std::move(ant1player1);
            auto ant1player2 = PieceFactory::createPiece(enums::PieceType::ANT);
            const std::shared_ptr ant1player2shared = std::move(ant1player2);

            // Create spider piece for each player
            auto spider1player1 = PieceFactory::createPiece(enums::PieceType::SPIDER);
            const std::shared_ptr spider1player1shared = std::move(spider1player1);
            auto spider1player2 = PieceFactory::createPiece(enums::PieceType::SPIDER);
            const std::shared_ptr spider1player2shared = std::move(spider1player2);

            // Create beetle piece for each player
            auto beetle1player1 = PieceFactory::createPiece(enums::PieceType::BEETLE);
            const std::shared_ptr beetle1player1shared = std::move(beetle1player1);
            auto beetle1player2 = PieceFactory::createPiece(enums::PieceType::BEETLE);
            const std::shared_ptr beetle1player2shared = std::move(beetle1player2);

            // Create grasshopper piece for each player
            auto grasshopper1player1 = PieceFactory::createPiece(enums::PieceType::GRASSHOPPER);
            const std::shared_ptr grasshopper1player1shared = std::move(grasshopper1player1);
            auto grasshopper1player2 = PieceFactory::createPiece(enums::PieceType::GRASSHOPPER);
            const std::shared_ptr grasshopper1player2shared = std::move(grasshopper1player2);

            // Create ladybug piece for each player
            auto ladybug1player1 = PieceFactory::createPiece(enums::PieceType::LADYBUG);
            const std::shared_ptr ladybug1player1shared = std::move(ladybug1player1);
            auto ladybug1player2 = PieceFactory::createPiece(enums::PieceType::LADYBUG);
            const std::shared_ptr ladybug1player2shared = std::move(ladybug1player2);

            // Create mosquito piece for each player
            auto mosquito1player1 = PieceFactory::createPiece(enums::PieceType::MOSQUITO);
            const std::shared_ptr mosquito1player1shared = std::move(mosquito1player1);
            auto mosquito1player2 = PieceFactory::createPiece(enums::PieceType::MOSQUITO);
            const std::shared_ptr mosquito1player2shared = std::move(mosquito1player2);

            // Create pillbug piece for each player
            auto pillbug1player1 = PieceFactory::createPiece(enums::PieceType::PILLBUG);
            const std::shared_ptr pillbug1player1shared = std::move(pillbug1player1);
            auto pillbug1player2 = PieceFactory::createPiece(enums::PieceType::PILLBUG);
            const std::shared_ptr pillbug1player2shared = std::move(pillbug1player2);

            // Create queen piece for each player
            auto queen1player1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
            const std::shared_ptr queen1player1shared = std::move(queen1player1);
            auto queen1player2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
            const std::shared_ptr queen1player2shared = std::move(queen1player2);


            //==========================================================================================================

            //ant1player1 place on hex(0,0,0)
            const Move move1(player1, ant1player1shared, origine);
            move1.execute(board);
            turn++;

            std::cout << board.pieceCount() << std::endl;

            //ant1player2 place on hex(1,-1,0)
            const Move move2(player2, ant1player2shared, board.neighbor(origine, enums::Direction::EAST));
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
    TEST_F(StrategyTest, TestValidMoveWhenPieceBelongsToPlayer) {
        auto bee1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto bee2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr sharedBee1 = std::move(bee1);
        const std::shared_ptr sharedBee2 = std::move(bee2);

        player1->addPiece(sharedBee1);
        board.addPiece(Hex(0, 0, 0), sharedBee1);
        player2->addPiece(sharedBee2);
        board.addPiece(Hex(1, 0, -1), sharedBee2);

        const std::vector<Hex> possiblesMoves = sharedBee1->getMoveStrategy().getPossibleMoves(board, *player1);

        ASSERT_EQ(possiblesMoves.size(), 2);
    }

    TEST_F(StrategyTest, TestNoMoveWhenPieceDoesNotBelongToPlayer) {
    }

    TEST_F(StrategyTest, TestNoMovesWhenBoardIsBlocked) {
    }
} // namespace hive::models::strategies

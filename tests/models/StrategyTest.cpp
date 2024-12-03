#include <gtest/gtest.h>
#include <models/PieceFactory.h>
#include <models/Player.h>

#include "models/Board.h"

namespace hive::models::strategies {
    class StrategyTest : public testing::Test {
    protected:
        Board board; /**< Board instance used in tests */
        std::shared_ptr<Player> player1; /**< Player 1 used in tests */
        std::shared_ptr<Player> player2; /**< Player 2 used in tests */

        // Setup method for initializing the players
        void SetUp() override {
            Player::resetPlayerNextId();
            player1 = std::make_shared<Player>("Player1");
            player2 = std::make_shared<Player>("Player2");
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

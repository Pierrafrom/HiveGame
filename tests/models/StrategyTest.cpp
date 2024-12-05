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
     * tests for ValidateMovingPossibility
     * ****************************************************************************************************************/
    //TODO test for validateMovingPossibility

    /*******************************************************************************************************************
     * tests for QueenBeeMoveStrategy
     * ****************************************************************************************************************/
    TEST_F(StrategyTest, TestValidMoveWhenPieceBelongsToPlayer) {
        const Hex origin(0, 0, 0);
        auto bee1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto bee2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        const std::shared_ptr sharedBee1 = std::move(bee1);
        const std::shared_ptr sharedBee2 = std::move(bee2);

        player1->addPiece(sharedBee1);
        board.addPiece(origin, sharedBee1);
        player2->addPiece(sharedBee2);
        board.addPiece(board.neighbor(origin, enums::Direction::EAST), sharedBee2);

        const std::vector<Hex> possibleMoves = sharedBee1->getMoveStrategy().getPossibleMoves(board, *player1);

        // Ensure the possible moves contain the expected positions and the size is 2
        const Hex expectedMove1 = board.neighbor(origin, enums::Direction::NORTH_EAST);
        const Hex expectedMove2 = board.neighbor(origin, enums::Direction::SOUTH_EAST);

        ASSERT_EQ(2, possibleMoves.size());
        ASSERT_TRUE(std::find(possibleMoves.begin(), possibleMoves.end(), expectedMove1) != possibleMoves.end());
        ASSERT_TRUE(std::find(possibleMoves.begin(), possibleMoves.end(), expectedMove2) != possibleMoves.end());
    }


    TEST_F(StrategyTest, TestNoMovesWhenBoardIsBlocked) {
        const Hex origin(0, 0, 0);
        auto bee1 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto bee2 = PieceFactory::createPiece(enums::PieceType::QUEEN_BEE);
        auto beetle = PieceFactory::createPiece(enums::PieceType::BEETLE);

        const std::shared_ptr sharedBee1 = std::move(bee1);
        const std::shared_ptr sharedBee2 = std::move(bee2);
        const std::shared_ptr sharedBeetle = std::move(beetle);

        // Ajout de la reine de joueur 1 au centre
        player1->addPiece(sharedBee1);
        board.addPiece(origin, sharedBee1);

        // Entoure la reine avec des pièces
        board.addPiece(board.neighbor(origin, enums::Direction::NORTH_EAST), sharedBee2);
        board.addPiece(board.neighbor(origin, enums::Direction::EAST), sharedBeetle);
        board.addPiece(board.neighbor(origin, enums::Direction::SOUTH_EAST), sharedBee2);
        board.addPiece(board.neighbor(origin, enums::Direction::SOUTH_WEST), sharedBee2);
        board.addPiece(board.neighbor(origin, enums::Direction::WEST), sharedBeetle);
        board.addPiece(board.neighbor(origin, enums::Direction::NORTH_WEST), sharedBee2);

        // Obtient les mouvements possibles pour la reine bloquée
        const std::vector<Hex> possibleMoves = sharedBee1->getMoveStrategy().getPossibleMoves(board, *player1);

        // Assure qu'aucun mouvement n'est possible
        ASSERT_TRUE(possibleMoves.empty());
    }

    //TODO test every strategy for every piece
} // namespace hive::models::strategies

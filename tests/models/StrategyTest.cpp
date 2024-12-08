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

    /*******************************************************************************************************************
     * tests for SpiderStrategy
     * ****************************************************************************************************************/
    TEST_F(StrategyTest, TestNoMovesWhenSpiderIsFullySurounded) {
        const Hex origin(0, 0, 0);
        auto spider1 = PieceFactory::createPiece(enums::PieceType::SPIDER);
        auto beetle = PieceFactory::createPiece(enums::PieceType::BEETLE);

        const std::shared_ptr sharedSpider1 = std::move(spider1);
        const std::shared_ptr sharedBeetle = std::move(beetle);

        // Ajout de la spider du joueur 1 au centre
        player1->addPiece(sharedSpider1);
        board.addPiece(origin, sharedSpider1);

        // Entoure la spider avec des pièces
        board.addPiece(board.neighbor(origin, enums::Direction::NORTH_EAST), sharedBeetle);
        board.addPiece(board.neighbor(origin, enums::Direction::EAST), sharedBeetle);
        board.addPiece(board.neighbor(origin, enums::Direction::SOUTH_EAST), sharedBeetle);
        board.addPiece(board.neighbor(origin, enums::Direction::SOUTH_WEST), sharedBeetle);
        board.addPiece(board.neighbor(origin, enums::Direction::WEST), sharedBeetle);
        board.addPiece(board.neighbor(origin, enums::Direction::NORTH_WEST), sharedBeetle);

        // Obtient les mouvements possibles pour la reine bloquée
        const std::vector<Hex> possibleMoves = sharedSpider1->getMoveStrategy().getPossibleMoves(board, *player1);

        // Assure qu'aucun mouvement n'est possible
        ASSERT_TRUE(possibleMoves.empty());
    }

    TEST_F(StrategyTest, TestValidMoveSpiderWithBacktrack) {
        const Hex origin(0, 0, 0);

        auto spider1 = PieceFactory::createPiece(enums::PieceType::SPIDER);
        auto beetle = PieceFactory::createPiece(enums::PieceType::BEETLE);

        const std::shared_ptr sharedSpider1 = std::move(spider1);
        const std::shared_ptr sharedBeetle = std::move(beetle);

        // Ajout de la spider du joueur 1 au centre
        player1->addPiece(sharedSpider1);
        board.addPiece(origin, sharedSpider1);

        // Entoure partiellement la spider avec des pièces
        board.addPiece(board.neighbor(origin, enums::Direction::NORTH_EAST), sharedBeetle);
        board.addPiece(board.neighbor(origin, enums::Direction::EAST), sharedBeetle);
        board.addPiece(board.neighbor(origin, enums::Direction::SOUTH_EAST), sharedBeetle);
        board.addPiece(board.neighbor(origin, enums::Direction::SOUTH_WEST), sharedBeetle);
        board.addPiece(board.neighbor(origin, enums::Direction::WEST), sharedBeetle);

        // Laisse un espace libre au nord-ouest
        // Cela permettra de tester le backtracking de la spider.

        // Obtient les mouvements possibles pour la spider
        const std::vector<Hex> possibleMoves = sharedSpider1->getMoveStrategy().getPossibleMoves(board, *player1);

        // Vérifie que le nombre de mouvements est correct
        ASSERT_EQ(possibleMoves.size(), 11);
    }
    /*******************************************************************************************************************
     * tests for GrasshopperMoveStrategy
     * ****************************************************************************************************************/
    TEST_F(StrategyTest, TestNoMovesWhenGrasshoperIsnotFullySuroundedAndNoPiecesToJump) {
        const Hex origin(0, 0, 0);
        auto grasshopper = PieceFactory::createPiece(enums::PieceType::GRASSHOPPER);
        auto beetle = PieceFactory::createPiece(enums::PieceType::BEETLE);

        const std::shared_ptr sharedGrasshopper = std::move(grasshopper);
        const std::shared_ptr sharedBeetle = std::move(beetle);

        // Ajout de la spider du joueur 1 au centre
        player1->addPiece(sharedGrasshopper);
        board.addPiece(origin, sharedGrasshopper);

        // Entoure le grasshopper avec deux pièces
        board.addPiece(board.neighbor(origin, enums::Direction::NORTH_EAST), sharedBeetle);
        board.addPiece(board.neighbor(origin, enums::Direction::SOUTH_EAST), sharedBeetle);

        // Obtient les mouvements possibles pour le grasshopper
        const std::vector<Hex> possibleMoves = sharedGrasshopper->getMoveStrategy().getPossibleMoves(board, *player1);

        // Vérifie que le nombre de mouvements est correct
        ASSERT_TRUE(possibleMoves.empty());
    }
    TEST_F(StrategyTest, TestValidGrasshopperMoveStrategy) {
        const Hex origin(0, 0, 0);
        auto grasshopper = PieceFactory::createPiece(enums::PieceType::GRASSHOPPER);
        auto beetle = PieceFactory::createPiece(enums::PieceType::BEETLE);

        const std::shared_ptr sharedGrasshopper = std::move(grasshopper);
        const std::shared_ptr sharedBeetle = std::move(beetle);

        // Ajout de la spider du joueur 1 au centre
        player1->addPiece(sharedGrasshopper);
        board.addPiece(origin, sharedGrasshopper);

        // Entoure le grasshopper avec deux pièces
        board.addPiece(board.neighbor(origin, enums::Direction::NORTH_EAST), sharedBeetle);
        board.addPiece(board.neighbor(origin, enums::Direction::SOUTH_EAST), sharedBeetle);
        board.addPiece(board.neighbor(origin, enums::Direction::EAST), sharedBeetle);

        // Obtient les mouvements possibles pour le grasshopper
        const std::vector<Hex> possibleMoves = sharedGrasshopper->getMoveStrategy().getPossibleMoves(board, *player1);

        // Vérifie que le nombre de mouvements est correct
        ASSERT_EQ(possibleMoves.size(), 3);
    }
    TEST_F(StrategyTest, TestValidGrasshopperMoveStrategyToTheFirstValidEmplacementInADirection) {
        const Hex origin(0, 0, 0);
        auto grasshopper = PieceFactory::createPiece(enums::PieceType::GRASSHOPPER);
        auto beetle = PieceFactory::createPiece(enums::PieceType::BEETLE);

        const std::shared_ptr sharedGrasshopper = std::move(grasshopper);
        const std::shared_ptr sharedBeetle = std::move(beetle);

        // Ajout de la spider du joueur 1 au centre
        player1->addPiece(sharedGrasshopper);
        board.addPiece(origin, sharedGrasshopper);

        // Créer une configuration de plateau où il y a deux emplacement valide au nord-est pour tester si le déplacement se fait bien au premier emplacement valide
        board.addPiece(board.neighbor(origin, enums::Direction::NORTH_EAST), sharedBeetle);
        board.addPiece(board.neighbor(origin, enums::Direction::EAST), sharedBeetle);
        board.addPiece(board.neighbor(board.neighbor(origin, enums::Direction::EAST), enums::Direction::NORTH_EAST), sharedBeetle);
        board.addPiece(board.neighbor(board.neighbor(board.neighbor(origin, enums::Direction::EAST), enums::Direction::NORTH_EAST), enums::Direction::NORTH_EAST), sharedBeetle);


        // Obtient les mouvements possibles pour le grasshopper
        const std::vector<Hex> possibleMoves = sharedGrasshopper->getMoveStrategy().getPossibleMoves(board, *player1);

        // Vérifie que le nombre de mouvements est correct
        ASSERT_EQ(possibleMoves.size(), 2);
        // Vérifie que le déplacement se fait bien au premier emplacement valide
        ASSERT_EQ(possibleMoves[0], board.neighbor(board.neighbor(origin, enums::Direction::NORTH_EAST), enums::Direction::NORTH_EAST));
    }
    /*******************************************************************************************************************
     * tests for BeetleMoveStrategy
     * ****************************************************************************************************************/
    TEST_F(StrategyTest, TestValidBeetleMoveStrategyWhenIsAlmosSurounded) {
        const Hex origin(0, 0, 0);
        auto beetle = PieceFactory::createPiece(enums::PieceType::BEETLE);
        auto spider = PieceFactory::createPiece(enums::PieceType::SPIDER);

        const std::shared_ptr sharedBeetle = std::move(beetle);
        const std::shared_ptr sharedSpider = std::move(spider);

        // Ajout du beetle du joueur 1 au centre
        player1->addPiece(sharedBeetle);
        board.addPiece(origin, sharedBeetle);

        // Entoure le beetle avec deux pièces
        board.addPiece(board.neighbor(origin, enums::Direction::NORTH_EAST), sharedSpider);
        board.addPiece(board.neighbor(origin, enums::Direction::EAST), sharedSpider);
        board.addPiece(board.neighbor(origin, enums::Direction::SOUTH_EAST), sharedSpider);
        board.addPiece(board.neighbor(origin, enums::Direction::SOUTH_WEST), sharedSpider);
        board.addPiece(board.neighbor(origin, enums::Direction::WEST), sharedSpider);


        // Obtient les mouvements possibles pour le beetle
        const std::vector<Hex> possibleMoves = sharedBeetle->getMoveStrategy().getPossibleMoves(board, *player1);

        // Vérifie que le nombre de mouvements est correct
        ASSERT_EQ(possibleMoves.size(), 6);
    }
    TEST_F(StrategyTest, TestOneMoveWhenBeetleBreakTheHive) {
        const Hex origin(0, 0, 0);
        auto beetle = PieceFactory::createPiece(enums::PieceType::BEETLE);
        auto spider = PieceFactory::createPiece(enums::PieceType::SPIDER);

        const std::shared_ptr sharedBeetle = std::move(beetle);
        const std::shared_ptr sharedSpider = std::move(spider);

        // Ajout du beetle du joueur 1 au centre
        player1->addPiece(sharedBeetle);
        board.addPiece(origin, sharedBeetle);

        // Entoure le beetle avec deux pièces
        board.addPiece(board.neighbor(origin, enums::Direction::NORTH_EAST), sharedSpider);
        board.addPiece(board.neighbor(origin, enums::Direction::SOUTH_EAST), sharedSpider);


        // Obtient les mouvements possibles pour le beetle
        const std::vector<Hex> possibleMoves = sharedBeetle->getMoveStrategy().getPossibleMoves(board, *player1);

        // Vérifie que le nombre de mouvements est correct
        ASSERT_EQ(possibleMoves.size(), 1);
    }

    /*******************************************************************************************************************
     * tests for AntMoveStrategy
     * ****************************************************************************************************************/
    TEST_F(StrategyTest, TestNoMovesWhenAntIsFullySurounded) {
        const Hex origin(0,0,0);

        auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        auto spider = PieceFactory::createPiece(enums::PieceType::SPIDER);

        const std::shared_ptr sharedAnt = std::move(ant);
        const std::shared_ptr sharedSpider = std::move(spider);

        player1->addPiece(sharedAnt);
        board.addPiece(origin, sharedAnt);

        // Entoure la Ant partillement
        board.addPiece(board.neighbor(origin, enums::Direction::NORTH_EAST), sharedSpider);
        board.addPiece(board.neighbor(origin, enums::Direction::EAST), sharedSpider);
        board.addPiece(board.neighbor(origin, enums::Direction::SOUTH_EAST), sharedSpider);
        board.addPiece(board.neighbor(origin, enums::Direction::SOUTH_WEST), sharedSpider);
        board.addPiece(board.neighbor(origin, enums::Direction::WEST), sharedSpider);
        board.addPiece(board.neighbor(origin, enums::Direction::NORTH_WEST), sharedSpider);

        // Obtient les mouvements possibles pour le beetle
        const std::vector<Hex> possibleMoves = sharedAnt->getMoveStrategy().getPossibleMoves(board, *player1);

        // Vérifie que le nombre de mouvements est correct
        ASSERT_TRUE(possibleMoves.empty());
    }

    TEST_F(StrategyTest, TestMoveSWhenAntCanPassTroughTheSpace) {
        const Hex origin(0,0,0);

        auto ant = PieceFactory::createPiece(enums::PieceType::ANT);
        auto spider = PieceFactory::createPiece(enums::PieceType::SPIDER);

        const std::shared_ptr sharedAnt = std::move(ant);
        const std::shared_ptr sharedSpider = std::move(spider);

        player1->addPiece(sharedAnt);
        board.addPiece(origin, sharedAnt);

        // Entoure la Ant partillement
        board.addPiece(board.neighbor(origin, enums::Direction::NORTH_WEST), sharedSpider);
        board.addPiece(board.neighbor(origin, enums::Direction::NORTH_EAST), sharedSpider);
        board.addPiece(board.neighbor(origin, enums::Direction::EAST), sharedSpider);
        board.addPiece(board.neighbor(origin, enums::Direction::SOUTH_EAST), sharedSpider);

        // Obtient les mouvements possibles pour le beetle
        const std::vector<Hex> possibleMoves = sharedAnt->getMoveStrategy().getPossibleMoves(board, *player1);

        // Vérifie que le nombre de mouvements est correct
        //ASSERT_EQ(possibleMoves.size(), 11);
    }

    //TODO test every strategy for every piece
} // namespace hive::models::strategies

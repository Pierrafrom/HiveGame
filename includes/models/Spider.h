#ifndef HIVEGAME_SPIDER_H
#define HIVEGAME_SPIDER_H
#include "Piece.h"
#include "MoveStrategy.h"

namespace hive::models{
    class Spider : public Piece {
    public:
        Spider(const size_t id, const Player* player, const Hex& posInitiale) : Piece(id, player, posInitiale){
            this->setMoveStrategy(new MoveStrategy());
        }


    };
}

#endif //HIVEGAME_SPIDER_H

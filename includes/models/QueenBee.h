#ifndef HIVEGAME_QUEENBEE_H
#define HIVEGAME_QUEENBEE_H
#include "Piece.h"
#include "MoveStrategy.h"

namespace hive::models{
    class QueenBee : public Piece {
    public:
        QueenBee(const size_t id, const Player* player, const Hex& posInitiale) : Piece(id, player, posInitiale){
            this->setMoveStrategy(new MoveStrategy());
        }


    };
}

#endif //HIVEGAME_QUEENBEE_H

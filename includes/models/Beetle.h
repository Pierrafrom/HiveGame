#ifndef HIVEGAME_BEETLE_H
#define HIVEGAME_BEETLE_H

#include "Piece.h"
#include "MoveStrategy.h"

namespace hive::models{
    class Beetle : public Piece {
    public:
        Beetle(const size_t id, const Player* player, const Hex& posInitiale) : Piece(id, player, posInitiale){
            this->setMoveStrategy(new MoveStrategy());
        }


    };
}

#endif //HIVEGAME_BEETLE_H

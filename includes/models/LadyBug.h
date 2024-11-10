#ifndef HIVEGAME_LADYBUG_H
#define HIVEGAME_LADYBUG_H

#include "Piece.h"
#include "MoveStrategy.h"

namespace hive::models{
    class LadyBug : public Piece {
    public:
        LadyBug(const size_t id, const Player* player, const Hex& posInitiale) : Piece(id, player, posInitiale){
            this->setMoveStrategy(new MoveStrategy());
        }


    };
}

#endif //HIVEGAME_LADYBUG_H

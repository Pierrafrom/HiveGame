#ifndef HIVEGAME_ANT_H
#define HIVEGAME_ANT_H
#include "Piece.h"
#include "MoveStrategy.h"

namespace hive::models{
    class Ant : public Piece {
    public:
        Ant(const size_t id, const Player* player, const Hex& posInitiale) : Piece(id, player, posInitiale){
            this->setMoveStrategy(new MoveStrategy());
        }


    };
}

#endif //HIVEGAME_ANT_H

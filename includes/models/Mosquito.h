#ifndef HIVEGAME_MOSQUITO_H
#define HIVEGAME_MOSQUITO_H

#include "Piece.h"
#include "MoveStrategy.h"

namespace hive::models{
    class Mosquito : public Piece {
    public:
        Mosquito(const size_t id, const Player* player, const Hex& posInitiale) : Piece(id, player, posInitiale){
            this->setMoveStrategy(new MoveStrategy());
        }


    };
}

#endif //HIVEGAME_MOSQUITO_H

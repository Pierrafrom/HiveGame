#ifndef HIVEGAME_PILLBUG_H
#define HIVEGAME_PILLBUG_H
#include "Piece.h"
#include "MoveStrategy.h"

namespace hive::models{
    class Pillbug : public Piece {
    public:
        Pillbug(const size_t id, const Player* player, const Hex& posInitiale) : Piece(id, player, posInitiale){
            this->setMoveStrategy(new MoveStrategy());
        }

    };
}

#endif //HIVEGAME_PILLBUG_H
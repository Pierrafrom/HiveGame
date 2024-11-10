//
#ifndef HIVEGAME_GRASSHOPPER_H
#define HIVEGAME_GRASSHOPPER_H
#include "Piece.h"
#include "MoveStrategy.h"

namespace hive::models{
    class Grasshopper : public Piece {
    public:
        Grasshopper(const size_t id, const Player* player, const Hex& posInitiale) : Piece(id, player, posInitiale){
            this->setMoveStrategy(new MoveStrategy());
        }


    };
}

#endif //HIVEGAME_GRASSHOPPER_H

#include <vector>
#include "models/Piece.h"

namespace hive::models {

    void Piece::setMoveStrategy(MoveStrategy *strategy) {
        this->moveStrategy = strategy;
    }

    /*void Piece::setPosition(const Hex &newPosition) {
        this->position = newPosition;
    }*/

    void Piece::immobilize() {
        this->isImmobilized = true;
    }

    void Piece::release() {
        this->isImmobilized = false;
    }

    Piece::~Piece() {
        delete moveStrategy;
    }
}

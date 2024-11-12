#include "models/Piece.h"

namespace hive::models {
    size_t Piece::nextId = 1; // Initialize static ID counter

    // Constructor implementation
    Piece::Piece(const size_t id, const enums::PieceType type,
                 std::unique_ptr<strategies::MoveStrategy> strategy): id(id), type(type),
                                                                      moveStrategy(std::move(strategy)) {
        if (id >= nextId) {
            nextId = id + 1;
        }
    }

    // Get the movement strategy of the piece and throw an exception if it is not set
    const strategies::MoveStrategy &Piece::getMoveStrategy() const {
        if (moveStrategy == nullptr) {
            throw std::runtime_error("Move strategy is not set");
        }
        return *moveStrategy;
    }
}

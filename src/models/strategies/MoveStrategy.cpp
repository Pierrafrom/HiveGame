#include "models/strategies/MoveStrategy.h"

namespace hive::models::strategies {
    // Retrieves the piece that uses this movement strategy
    const Piece &MoveStrategy::getPiece() const {
        if (!piece) {
            throw std::invalid_argument("Piece cannot be null.");
        }
        return *piece;
    }
} // namespace hive::models::strategies

#include "models/strategies/LadybugMoveStrategy.h"

namespace hive::models::strategies {
    std::vector<Hex> LadybugMoveStrategy::getPossibleMoves(const Hex &position,
                                                           const Board &board,
                                                           const Player &player) const {
        // TODO: Implement Ladybug movement logic: two spaces on top of other pieces, one space on an open position
        return {}; // PLACEHOLDER
    }
} // namespace hive::models::strategies

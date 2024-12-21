#include "models/strategies/PillbugMoveStrategy.h"
#include <models/Board.h>
#include <models/enums/Direction.h>
#include <models/Player.h>
#include <vector>

namespace hive::models::strategies {
    std::vector<Hex> PillbugMoveStrategy::getPossibleMoves(const Board &board, const Player &player) const {
        // TODO: Implement Mosquito movement logic by imitating adjacent pieces' movement abilities
        return {}; // PLACEHOLDER
    }

    std::vector<Hex> PillbugMoveStrategy::getPossibleMoves(const Board &board, const Player &player, const Hex &hex) const {
        // TODO: Implement Mosquito movement logic by imitating adjacent pieces' movement abilities
        return {}; // PLACEHOLDER
    }
} // namespace hive::models::strategies

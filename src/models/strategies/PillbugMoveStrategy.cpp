#include "models/strategies/PillbugMoveStrategy.h"
#include <models/Board.h>
#include <models/enums/Direction.h>
#include <models/Player.h>
#include <vector>

namespace hive::models::strategies {
    std::vector<Hex> PillbugMoveStrategy::getPossibleMoves(const Board &board, const Player &player) const {

        if (!validatePieceOwnership(player)) {
            return {};
        }

        const Hex position = this->getPiece().getPosition().value();

        std::vector<Hex> possibleMoves{};

        for (const auto &direction: enums::getAllDirections()) {
            const bool canSlice = board.canSliceBetween(position, direction);
            const bool connected = isHiveConnectedAfterMove(board, position, board.neighbor(position, direction));
            if (canSlice && connected) {
                possibleMoves.push_back(board.neighbor(position, direction));
            }
        }

        return possibleMoves;
    }
} // namespace hive::models::strategies

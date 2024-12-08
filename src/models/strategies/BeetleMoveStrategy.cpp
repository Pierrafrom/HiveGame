#include "models/strategies/BeetleMoveStrategy.h"

#include <models/Board.h>

namespace hive::models::strategies {
    std::vector<Hex> BeetleMoveStrategy::getPossibleMoves(const Board &board, const Player &player) const {

        if (!validatePieceOwnership(player)) {
            return {};
        }

        const Hex position = this->getPiece().getPosition().value();

        std::vector<Hex> possibleMoves{};

        for (const auto &direction: enums::getAllDirections()) {
            const bool connected = isHiveConnectedAfterMove(board, position, board.neighbor(position, direction));
            if (connected) {
                possibleMoves.push_back(board.neighbor(position, direction));
            }
        }

        return possibleMoves;

    }
} // namespace hive::models::strategies

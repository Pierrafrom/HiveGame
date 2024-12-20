#include "models/strategies/QueenBeeMoveStrategy.h"

#include <models/Board.h>
#include <models/enums/Direction.h>

namespace hive::models::strategies {
    std::vector<Hex> QueenBeeMoveStrategy::getPossibleMoves(const Board &board, const Player &player) const {

        if (!validatePieceOwnership(player)) {
            return {};
        }

        const Hex position = this->getPiece().getPosition().value();

        std::vector<Hex> possibleMoves{};

        for (const auto &direction: enums::getAllDirections()) {
            Hex neighbor{0, 0, 0}; // O,0,0 is a placeholder to create a hex object
            try {
                neighbor = board.neighbor(position, direction);
            } catch (const std::out_of_range &e) {
                continue;
            } catch (const std::exception &e) {
                throw e;
            }
            const bool occupied = board.isOccupied(neighbor);
            const bool canSlice = board.canSliceBetween(position, direction);
            //const bool connected = isHiveConnectedAfterMove(board, position, neighbor);
            if (!occupied && canSlice) {

                possibleMoves.push_back(neighbor);

            }
        }

        return possibleMoves;
    }
} // namespace hive::models::strategies

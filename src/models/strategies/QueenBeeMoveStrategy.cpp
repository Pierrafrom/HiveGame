#include "models/strategies/QueenBeeMoveStrategy.h"

#include <models/Board.h>
#include <models/enums/Direction.h>

namespace hive::models::strategies {
    std::vector<Hex> QueenBeeMoveStrategy::getPossibleMoves(const Board &board, const Player &player) const {
        const Hex position = this->getPiece().getPosition().value();
        if (!this->hasPiece()) {
            throw std::runtime_error("Move strategy does not have a piece.");
        }
        if (!this->getPiece().hasOwner()) {
            throw std::runtime_error("Piece does not have an owner.");
        }
        if (this->getPiece().getOwner() != player) {
            // Return empty vector because there is movement available for a piece that does not belong to the player
            return {};
        }
        std::vector<Hex> possibleMoves{};
        for (const auto &direction: enums::getAllDirections()) {
            const bool occupied = board.isOccupied(board.neighbor(position, direction));
            const bool canSlice = board.canSliceBetween(position, direction);
            const bool connected = isHiveConnectedAfterMove(board, position, board.neighbor(position, direction));
            if (!occupied && canSlice && connected) {
                possibleMoves.push_back(board.neighbor(position, direction));
            }
        }

        return possibleMoves;
    }
} // namespace hive::models::strategies

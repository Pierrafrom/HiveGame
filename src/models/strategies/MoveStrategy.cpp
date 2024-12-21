#include "models/strategies/MoveStrategy.h"

#include "models/Player.h"
#include <models/Board.h>

namespace hive::models::strategies {
    // Check if the hive is still connected after moving a piece
    bool MoveStrategy::isHiveConnectedAfterMove(Board board, Hex from, Hex to) {
        board.movePiece(from, to);
        return board.areAllPiecesConnected();
    }

    // Retrieves the piece that uses this movement strategy
    const Piece &MoveStrategy::getPiece() const {
        if (!piece) {
            throw std::invalid_argument("Piece cannot be null.");
        }
        return *piece;
    }

    bool MoveStrategy::validatePieceOwnership(const Player &player) const {
        if (!this->hasPiece()) {
            throw std::runtime_error("Move strategy does not have a piece.");
        }
        if (!this->getPiece().hasOwner()) {
            throw std::runtime_error("Piece does not have an owner.");
        }
        if (this->getPiece().getOwner() != player) {
            return false;
        }
        return true;
    }

} // namespace hive::models::strategies

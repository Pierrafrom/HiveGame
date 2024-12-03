#include "models/strategies/MoveStrategy.h"

#include <models/Board.h>

namespace hive::models::strategies {
    // Check if the hive is still connected after moving a piece
    bool MoveStrategy::isHiveConnectedAfterMove(Board board, const Hex &from, const Hex &to) {
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
} // namespace hive::models::strategies

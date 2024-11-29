#include "models/Move.h"
#include "models/Board.h"
#include <stdexcept>

namespace hive::models {
    // Constructor for PLACE move
    Move::Move(Player* player, Piece* piece, const Hex& to)
        : type(MoveType::PLACE), player(player), piece(piece), from(Hex(0, 0, 0)), to(to) {
        if (!this->piece) {
            throw std::invalid_argument("Piece cannot be null for a PLACE move.");
        }
    }

    // Constructor for MOVE move
    Move::Move(Player* player, Piece* piece, const Hex& from, const Hex& to)
        : type(MoveType::MOVE), player(player), piece(piece), from(from), to(to) {
        if (!piece) {
            throw std::invalid_argument("Piece cannot be null for a MOVE move.");
        }
    }

    // Executes the move
    void Move::execute(Board &board) const {
        //! WARNING: Before executing the move, ensure that the move is valid by calling validateMove()
        //! or validatePlacement() from GameRules
        if (type == MoveType::PLACE) {
            // For PLACE move: Add the piece to the target position
            board.addPiece(to, piece);
        } else if (type == MoveType::MOVE) {
            // For MOVE move: Move the piece from the starting position to the target position
            board.movePiece(from, to);
        } else {
            throw std::runtime_error("Invalid move type.");
        }
    }

    // Undoes the move
    void Move::undo(Board &board) const {
        if (type == MoveType::PLACE) {
            // For PLACE move: Remove the piece from the target position
            board.unstackPiece(to);
        } else if (type == MoveType::MOVE) {
            // For MOVE move: Move the piece back to the starting position
            board.movePiece(to, from);
        } else {
            throw std::runtime_error("Invalid move type.");
        }
    }

    // Retrieves the starting position of the piece
    const Hex &Move::getFrom() const {
        if (type != MoveType::MOVE) {
            throw std::runtime_error("Starting position is only valid for MOVE moves.");
        }
        return from;
    }
} // namespace hive::models

#include "models/Move.h"
#include <stdexcept>
#include <sstream>
#include <utility>

namespace hive::models {
    /**************************************************************************************************
     * Constructors
     *************************************************************************************************/

    // Constructor for PLACE move
    Move::Move(const std::shared_ptr<Player> &player,
               const std::shared_ptr<Piece> &piece,
               Hex to)
        : type(MoveType::PLACE),
          player(player),
          piece(piece),
          from(std::nullopt),
          to(std::move(to)) {
        if (!player) {
            throw std::invalid_argument("Player cannot be null for a MOVE.");
        }
        if (!piece) {
            throw std::invalid_argument("Piece cannot be null for a MOVE.");
        }
    }

    // Constructor for MOVE move
    Move::Move(const std::shared_ptr<Player> &player,
               const std::shared_ptr<Piece> &piece,
               const Hex &from,
               Hex to)
        : type(MoveType::MOVE),
          player(player),
          piece(piece),
          from(from),
          to(std::move(to)) {
        if (!player) {
            throw std::invalid_argument("Player cannot be null for a MOVE.");
        }
        if (!piece) {
            throw std::invalid_argument("Piece cannot be null for a MOVE.");
        }
    }

    /**************************************************************************************************
     * Getters
     *************************************************************************************************/

    // Retrieves the player who made the move
    std::shared_ptr<Player> Move::getPlayer() const {
        auto lockedPlayer = player.lock();
        if (!lockedPlayer) {
            throw std::runtime_error("Player associated with the move is no longer valid.");
        }
        return lockedPlayer;
    }

    // Retrieves the piece involved in the move
    std::shared_ptr<Piece> Move::getPiece() const {
        auto lockedPiece = piece.lock();
        if (!lockedPiece) {
            throw std::runtime_error("Piece associated with the move is no longer valid.");
        }
        return lockedPiece;
    }

    /**************************************************************************************************
     * Public Methods
     *************************************************************************************************/

    // Executes the move
    void Move::execute(Board &board) const {
        const auto piecePtr = getPiece();
        if (!piecePtr) {
            throw std::runtime_error("Piece is null in the move.");
        }

        if (type == MoveType::PLACE) {
            // Cast to non-const pointer for Board
            board.addPiece(to, piecePtr);
            // If the piece has no owner, add it to the player
            if (!piecePtr->hasOwner()) {
                if (const auto playerPtr = getPlayer(); playerPtr) {
                    getPlayer()->addPiece(piecePtr);
                } else {
                    throw std::runtime_error("Player is null in the move.");
                }
            }
        } else if (type == MoveType::MOVE) {
            if (!from) {
                throw std::runtime_error("From position is not set for a MOVE.");
            }
            board.movePiece(*from, to);
        } else {
            throw std::runtime_error("Invalid move type.");
        }
    }

    // Undoes the move
    void Move::undo(Board &board) const {
        const auto piecePtr = getPiece();
        if (!piecePtr) {
            throw std::runtime_error("Piece is null in the move.");
        }

        if (type == MoveType::PLACE) {
            // Remove the piece from the board
            if (const auto unstackedPiece = board.unstackPiece(to); unstackedPiece != piecePtr) {
                throw std::runtime_error("Unstacked piece does not match the expected piece.");
            }
            // Remove the piece from the player collection
            if (const auto playerPtr = getPlayer(); playerPtr) {
                playerPtr->removePiece(piecePtr);
            } else {
                throw std::runtime_error("Player is null in the move.");
            }
            // Reset the piece's position
            piecePtr->resetPosition();
        } else if (type == MoveType::MOVE) {
            if (!from) {
                throw std::runtime_error("From position is not set for a MOVE.");
            }
            board.movePiece(to, *from);
        } else {
            throw std::runtime_error("Invalid move type.");
        }
    }

    // Converts the move to a string representation
    std::string Move::toString() const {
        std::ostringstream oss;
        oss << "Move(Type: " << (type == MoveType::PLACE ? "PLACE" : "MOVE")
                << ", Player: " << (player.expired() ? "Invalid" : std::to_string(player.lock()->getId()))
                << ", Piece: " << (piece.expired() ? "Invalid" : std::to_string(piece.lock()->getId()))
                << ", From: " << (from ? from->toString() : "None")
                << ", To: " << to.toString() << ")";
        return oss.str();
    }

    // Overloads the stream insertion operator for Move
    std::ostream &operator<<(std::ostream &os, const Move &move) {
        os << move.toString();
        return os;
    }
} // namespace hive::models

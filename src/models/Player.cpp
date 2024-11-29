#include "models/Player.h"

namespace hive::models {
    size_t Player::playerNextId = 1; // Initialize static ID counter

    // Add a piece to the player's collection
    void Player::addPiece(std::unique_ptr<Piece> piece) {
        pieces.push_back(std::move(piece));
    }

    // Get the number of pieces of a specific type that the player owns
    int Player::getPieceCount(const enums::PieceType type) const {
        int count = 0;
        for (const auto &piece: pieces) {
            if (piece->getType() == type) {
                count++;
            }
        }
        return count;
    }

    // Get the collection of pieces owned by the player of a specific type
    std::vector<const Piece *> Player::getPieces(const enums::PieceType type) const {
        std::vector<const Piece *> filteredPieces;

        for (const auto &piece: pieces) {
            if (piece->getType() == type) {
                filteredPieces.push_back(piece.get());
            }
        }

        return filteredPieces;
    }

    // Check if the player owns a specific piece
    bool Player::ownsPiece(const Piece &piece) const {
        for (const auto &ownedPiece: pieces) {
            if (ownedPiece.get() == &piece) {
                return true;
            }
        }
        return false;
    }
}

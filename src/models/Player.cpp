#include "models/Player.h"
#include "models/Piece.h"
#include <sstream>
#include <stdexcept>

namespace hive::models {
    size_t Player::playerNextId = 1; // Initialize static counter for player IDs

    /**************************************************************************************************
     * Constructors
     *************************************************************************************************/

    Player::Player(const size_t id, std::string name) : id(id), name(std::move(name)) {
        if (id >= playerNextId) {
            playerNextId = id + 1; // Update the next ID to avoid conflicts
        }
    }

    /**************************************************************************************************
     * Getters
     *************************************************************************************************/

    // Get the count of pieces owned by the player
    size_t Player::getPieceCount(const std::optional<enums::PieceType> type) const {
        size_t count = 0;

        for (const auto &weakPiece: pieces) {
            if (const auto piece = weakPiece.lock()) {
                // Check if the piece is still valid
                if (!type || piece->getType() == type) {
                    ++count;
                }
            }
        }

        return count;
    }

    // Get the pieces owned by the player (optionally filtered by type)
    std::vector<std::shared_ptr<Piece> > Player::getPieces(const std::optional<enums::PieceType> type) const {
        std::vector<std::shared_ptr<Piece> > filteredPieces;

        for (const auto &weakPiece: pieces) {
            if (auto piece = weakPiece.lock()) {
                if (!type || piece->getType() == *type) {
                    filteredPieces.push_back(piece);
                }
            }
        }

        return filteredPieces;
    }


    /**************************************************************************************************
     * Public Methods
     *************************************************************************************************/

    // Add a piece to the player's collection
    void Player::addPiece(const std::shared_ptr<Piece> &piece) {
        if (!piece) {
            throw std::invalid_argument("Cannot add a null piece.");
        }

        pieces.push_back(piece);
        piece->setOwner(std::weak_ptr(shared_from_this()));
    }


    // Check if the player owns a specific piece
    bool Player::ownsPiece(const std::shared_ptr<Piece> &piece) const {
        for (const auto &weakPiece: pieces) {
            if (auto ownedPiece = weakPiece.lock()) {
                // Check if the piece is still valid
                if (ownedPiece == piece) {
                    return true;
                }
            }
        }

        return false;
    }

    // Remove a piece from the player's collection
    void Player::removePiece(const std::shared_ptr<Piece> &piece) {
        if (!piece) {
            throw std::invalid_argument("Cannot remove a null piece.");
        }

        const auto it = std::remove_if(pieces.begin(), pieces.end(),
                                       [&piece](const std::weak_ptr<Piece> &weakPiece) {
                                           if (const auto ownedPiece = weakPiece.lock()) {
                                               return ownedPiece == piece;
                                           }
                                           return false;
                                       });

        pieces.erase(it, pieces.end());
    }

    // Convert the Player object to a string representation
    std::string Player::toString() const {
        std::ostringstream oss;
        oss << "Player(ID: " << id
                << ", Name: " << name
                << ", Pieces: [";

        const auto ownedPieces = getPieces();
        for (size_t i = 0; i < ownedPieces.size(); ++i) {
            oss << "Piece(ID: " << ownedPieces[i]->getId()
                    << ", Type: " << static_cast<int>(ownedPieces[i]->getType()) << ")";
            if (i < ownedPieces.size() - 1) {
                oss << ", ";
            }
        }

        oss << "])";
        return oss.str();
    }

    // Operator for printing the Player object to an output stream
    std::ostream &operator<<(std::ostream &os, const Player &player) {
        return os << player.toString();
    }
} // namespace hive::models

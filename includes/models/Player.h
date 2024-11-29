#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <memory>
#include "models/Piece.h"
#include "models/enums/PieceType.h"

//TODO: may add some attributes for the player class (name for example)
// TODO: Create a test suite for the Player class

namespace hive::models {
    /**
     * @class Player
     * @brief Represents a player in the Hive game.
     *
     * The Player class manages the pieces owned by the player, tracks their count, and provides
     * interactions with the game logic.
     */
    class Player {
        static size_t playerNextId; /**< Static counter for generating unique player identifiers */
        size_t id; /**< Unique identifier for the player */
        std::vector<std::unique_ptr<Piece> > pieces; /**< Pieces owned by the player */

    public:
        /**
         * @brief Constructs a Player with a unique identifier.
         * @param id Unique identifier for the player.
         */
        explicit Player(const size_t id) : id(id) {
        }

        /**
        * @brief Constructs a Player with a unique identifier.
        *
        * We use the playerNextId static variable to assign a unique identifier to the player.
        */
        Player() : id(playerNextId++) {
        }

        /**
         * @brief Destructor.
         */
        ~Player() = default;

        /**
         * @brief Adds a piece to the player's collection.
         * @param piece Unique pointer to the piece to add.
         *
         * This method takes ownership of the provided piece and adds it to the player's collection.
         */
        void addPiece(std::unique_ptr<Piece> piece);

        /**
         * @brief Checks the number of pieces of a specific type that the player owns.
         * @param type The type of piece to count.
         * @return The number of pieces of the specified type owned by the player.
         *
         * This method is used to track how many pieces of each type the player has, ensuring they
         * do not exceed the allowed limits specified in the game rules.
         */
        [[nodiscard]] int getPieceCount(enums::PieceType type) const;

        /**
         * @brief Gets the collection of pieces owned by the player.
         * @return Reference to the vector of pieces.
         *
         * The returned vector allows direct access to the player's pieces, which can be useful for
         * operations that require iterating through or modifying the pieces.
         */
        std::vector<std::unique_ptr<Piece> > &getPieces() { return pieces; }

        /**
         * @brief Gets the collection of pieces owned by the player without modification.
         * @return Const reference to the vector of pieces.
         *
         * The returned vector allows direct access to the player's pieces, which can be useful for
         * operations that require iterating through without modifying the pieces.
         */
        [[nodiscard]] const std::vector<std::unique_ptr<Piece> > &getPieces() const { return pieces; }

        /**
         * @brief Gets the collection of pieces owned by the player of a specific type.
         * @param type The type of pieces to filter (e.g., QUEEN_BEE, ANT).
         * @return A vector of pointers to the pieces of the specified type.
         *
         * This method filters the player's collection and returns only the pieces that
         * match the specified type. The returned vector is read-only and contains raw
         * pointers to the matching pieces for easier iteration.
         */
        [[nodiscard]] std::vector<const Piece *> getPieces(enums::PieceType type) const;

        /**
         * @brief Checks if the given piece belongs to the player.
         * @param piece The piece to check.
         * @return True if the piece belongs to the player, false otherwise.
         */
        [[nodiscard]] bool ownsPiece(const Piece &piece) const;

        /**
         * @brief Gets the player's unique identifier.
         * @return The player's identifier.
         */
        [[nodiscard]] size_t getId() const { return id; }

        /**
         * @brief Equality operator for comparing two Player objects.
         * @param other The other Player object to compare with.
         * @return True if the two players are considered equal, false otherwise.
         *
         * Players are considered equal if they have the same id.
         */
        bool operator==(const Player &other) const { return id == other.id; }

        /**
        * @brief Inequality operator for comparing two Player objects.
        * @param other The other Player object to compare with.
        * @return True if the two players are considered different, false otherwise.
        *
        * Players are considered different if they have different ids.
        */
        bool operator!=(const Player &other) const { return !(*this == other); }
    };
} // namespace hive::models

#endif // PLAYER_H

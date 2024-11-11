#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <memory>
#include "models/Piece.h"
#include "models/enums/PieceType.h"

//TODO: may add some attributes for the player class (name for example)
//TODO: autoincrement ?
// TODO: Create a test suite for the Player class
// TODO: Implement the Player class

namespace hive::models {
    /**
     * @class Player
     * @brief Represents a player in the Hive game.
     *
     * The Player class manages the pieces owned by the player, tracks their count, and provides
     * interactions with the game logic.
     */
    class Player {
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
         * @brief Gets the player's unique identifier.
         * @return The player's identifier.
         */
        [[nodiscard]] size_t getId() const { return id; }
    };
} // namespace hive::models

#endif // PLAYER_H

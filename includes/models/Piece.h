#ifndef PIECE_H
#define PIECE_H

#include <cstddef> // For size_t
#include <string>
#include <utility>


namespace hive::models {
    /**
     * @class Piece
     * @brief Represents a game piece in the Hive game.
     *
     * Each piece has a unique ID and possibly additional attributes
     * depending on the specific type of piece (e.g., Queen Bee, Beetle).
     */
    class Piece {
        size_t id; /**< Unique identifier for the piece */
        std::string type; /**< Type of the piece (e.g., "Queen", "Beetle") */

    public:
        /**
         * @brief Constructs a Piece with a given ID and type.
         * @param id The unique identifier for the piece.
         * @param type The type of the piece.
         */
        Piece(const size_t id, std::string type) : id(id), type(std::move(type)) {
        }

        /**
         * @brief Retrieves the ID of the piece.
         * @return The ID of the piece.
         */
        [[nodiscard]] size_t getId() const { return id; }

        /**
         * @brief Retrieves the type of the piece.
         * @return The type as a string.
         */
        [[nodiscard]] std::string getType() const { return type; }

        /**
         * @brief Equality operator for Piece.
         * @param other The other Piece to compare with.
         * @return True if both pieces have the same ID and type; otherwise, false.
         */
        bool operator==(const Piece &other) const {
            return id == other.id && type == other.type;
        }

        /**
         * @brief Inequality operator for Piece.
         * @param other The other Piece to compare with.
         * @return True if pieces are not equal; otherwise, false.
         */
        bool operator!=(const Piece &other) const {
            return !(*this == other);
        }
    };
} // namespace hive::models


#endif // PIECE_H

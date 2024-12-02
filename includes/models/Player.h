#ifndef PLAYER_H
#define PLAYER_H

#include <utility>
#include <vector>
#include <memory>
#include <optional>
#include "models/Piece.h"
#include "models/enums/PieceType.h"

namespace hive::models {
    /**
     * @class Player
     * @brief Represents a player in the Hive game.
     *
     * The Player class manages the pieces owned by the player, tracks their count, and provides
     * interactions with the game logic.
     */
    class Player : public std::enable_shared_from_this<Player> {
        /**************************************************************************************************
         * Attributes
         *************************************************************************************************/

        static size_t playerNextId; /**< Static counter for unique player IDs */
        size_t id; /**< Unique identifier for the player */
        std::string name; /**< Name of the player */
        std::vector<std::weak_ptr<Piece> > pieces; /**< Weak references to the pieces owned */

    public:
        /**************************************************************************************************
         * Constructors
         *************************************************************************************************/

        /**
         * @brief Constructs a Player with a unique identifier and optional name.
         * @param id Unique identifier for the player. Defaults to auto-incremented ID.
         * @param name Name of the player. Defaults to an empty string.
         */
        explicit Player(size_t id = playerNextId++, std::string name = "");

        /**
         * @brief Constructs a Player with a unique identifier and a name.
         * @param name Name of the player.
         */
        explicit Player(std::string name) : Player(playerNextId++, std::move(name)) {
        }

        /**
         * @brief Destructor.
         */
        ~Player() = default;

        /**************************************************************************************************
         * Other constructors and Operators
         *************************************************************************************************/

        /**
         * @brief Delete copy constructor.
         */
        Player(const Player &) = delete;

        /**
         * @brief Delete copy assignment operator.
         */
        Player &operator=(const Player &) = delete;

        /**
        * @brief Default move constructor.
        */
        Player(Player &&) noexcept = default;

        /**
         * @brief Default move assignment operator.
         */
        Player &operator=(Player &&) noexcept = default;

        /**************************************************************************************************
         * Getters
         *************************************************************************************************/

        /**
         * @brief Retrieves the player's unique identifier.
         * @return The player's identifier.
         */
        [[nodiscard]] size_t getId() const { return id; }

        /**
         * @brief Retrieves the player's name.
         * @return The player's name.
         */
        [[nodiscard]] const std::string &getName() const { return name; }

        /**
         * @brief Retrieves the total number of pieces owned by the player.
         * @param type Optional type to filter the count. Defaults to `std::nullopt` for all types.
         * @return The total count of pieces of the specified type, or all pieces if no type is provided.
         */
        [[nodiscard]] size_t getPieceCount(std::optional<enums::PieceType> type = std::nullopt) const;

        /**
         * @brief Retrieves pieces owned by the player.
         * @param type Optional type to filter the pieces. If no type is provided, all pieces are returned.
         * @return A vector of shared pointers to the pieces owned by the player.
         */
        [[nodiscard]] std::vector<std::shared_ptr<const Piece> > getPieces(
            std::optional<enums::PieceType> type = std::nullopt) const;

        /**************************************************************************************************
         * Setters
         *************************************************************************************************/

        /**
         * @brief Sets the player's name.
         * @param newName The new name to assign to the player.
         */
        void setName(const std::string &newName) { name = newName; }

        /**************************************************************************************************
         * Public Methods
         *************************************************************************************************/

        /**
         * @brief Adds a piece to the player's collection.
         * @param piece Shared pointer to the piece to add.
         */
        void addPiece(const std::shared_ptr<Piece> &piece);

        /**
         * @brief Checks if the player owns a specific piece.
         * @param piece Shared pointer to the piece to check.
         * @return True if the player owns the piece, false otherwise.
         */
        [[nodiscard]] bool ownsPiece(const std::shared_ptr<Piece> &piece) const;

        /**
         * @brief Removes a piece from the player's collection.
         * @param piece Shared pointer to the piece to remove.
         */
        void removePiece(const std::shared_ptr<Piece> &piece);

        /**
         * @brief Resets the playerNextId counter to 1.
         */
        static void resetPlayerNextId() { playerNextId = 1; }

        /**
         * @brief Converts the Player object to a string representation.
         * @return A string representation of the Player object.
         */
        [[nodiscard]] std::string toString() const;

        /**************************************************************************************************
         * Operators
         *************************************************************************************************/

        /**
         * @brief Equality operator for comparing two Player objects.
         * @param other The other Player object to compare with.
         * @return True if the two players are considered equal, false otherwise.
         *
         * Players are considered equal if they have the same ID.
         */
        bool operator==(const Player &other) const { return id == other.id; }

        /**
         * @brief Inequality operator for comparing two Player objects.
         * @param other The other Player object to compare with.
         * @return True if the two players are considered different, false otherwise.
         */
        bool operator!=(const Player &other) const { return !(*this == other); }

        /**
         * @brief Overloads the stream insertion operator for the Player class.
         * @param os The output stream.
         * @param player The Player object to output.
         * @return A reference to the output stream.
         */
        friend std::ostream &operator<<(std::ostream &os, const Player &player);
    };
} // namespace hive::models

#endif // PLAYER_H

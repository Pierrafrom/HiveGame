#ifndef PIECE_H
#define PIECE_H

#include <cstddef>
#include <memory>
#include <optional>

#include "models/enums/PieceType.h"
#include "models/strategies/MoveStrategy.h"

// TODO: Create a test suite for the Piece class

// Forward declaration of Player class
namespace hive::models {
    class Player;
}

namespace hive::models {
    /**
     * @class Piece
     * @brief Base class representing a generic game piece in the Hive game.
     *
     * This class serves as the foundation for all specific types of game pieces in Hive.
     * It contains attributes and methods that are common to all pieces, such as unique
     * identifiers, type information, and movement strategies.
     */
    class Piece {
    protected:
        /**************************************************************************************************
         * Attributes
         *************************************************************************************************/

        static size_t pieceNextId; /**< Static counter for auto-incrementing piece IDs */
        size_t id; /**< Unique identifier for the piece */
        enums::PieceType type; /**< The type of the piece (e.g., QUEEN_BEE, ANT) */
        std::unique_ptr<strategies::MoveStrategy> moveStrategy; /**< Movement strategy */
        std::weak_ptr<Player> owner; /**< Weak reference to the owning player */
        std::optional<Hex> position; /**< Current position on the board, can be empty */

        /**************************************************************************************************
         * Constructors
         *************************************************************************************************/

        /**
         * @brief Constructs a piece with optional attributes.
         * @param id Unique identifier for the piece.
         * @param type Type of the piece as defined in the PieceType enum.
         * @param strategy Movement strategy for the piece (default is nullptr).
         * @param owner Shared pointer to the owning player (default is nullptr).
         * @param position Optional position of the piece on the board (default is std::nullopt).
         */
        explicit Piece(size_t id,
                       enums::PieceType type,
                       std::unique_ptr<strategies::MoveStrategy> strategy = nullptr,
                       const std::shared_ptr<Player> &owner = nullptr,
                       const std::optional<Hex> &position = std::nullopt);

    public:
        /**************************************************************************************************
         * Destructor
         *************************************************************************************************/

        /**
         * @brief Virtual destructor.
         *
         * Ensures that derived classes can override and properly clean up resources if needed.
         */
        virtual ~Piece() = default;

        /**************************************************************************************************
         * Other constructors and operators
         *************************************************************************************************/

        /**
         * @brief Delete copy constructor.
         */
        Piece(const Piece &) = delete;

        /**
         * @brief Delete copy assignment operator.
         */
        Piece &operator=(const Piece &) = delete;

        /**
         * @brief Default move constructor.
         */
        Piece(Piece &&) noexcept;

        /**
         * @brief Default move assignment operator.
         */
        Piece &operator=(Piece &&) noexcept;

        /**************************************************************************************************
         * Getters
         *************************************************************************************************/

        /**
         * @brief Retrieves the unique identifier of the piece.
         * @return The unique ID of the piece.
         */
        [[nodiscard]] size_t getId() const { return id; }

        /**
         * @brief Retrieves the type of the piece.
         * @return The type of the piece as defined in the PieceType enum.
         */
        [[nodiscard]] enums::PieceType getType() const { return type; }

        /**
         * @brief Retrieves the movement strategy of the piece.
         * @return A reference to the MoveStrategy object.
         */
        [[nodiscard]] const strategies::MoveStrategy &getMoveStrategy() const;

        /**
         * @brief Retrieves the player that owns the piece.
         * @return A pointer to the player object that owns the piece.
         */
        [[nodiscard]] const Player &getOwner() const;

        /**
         * @brief Retrieves the current position of the piece.
         * @return An optional Hex representing the position of the piece.
         */
        [[nodiscard]] std::optional<Hex> getPosition() const { return position; }

        /**************************************************************************************************
         * Setters
         *************************************************************************************************/

        /**
         * @brief Sets the movement strategy for the piece.
         * @param strategy A unique pointer to the MoveStrategy object.
         */
        void setMoveStrategy(std::unique_ptr<strategies::MoveStrategy> strategy);

        /**
         * @brief Sets the player that owns the piece.
         * @param newOwner Shared pointer to the Player object.
         */
        void setOwner(const std::weak_ptr<Player> &newOwner);

        /**
         * @brief Sets the position of the piece on the board.
         * @param newPosition The new position of the piece as a Hex.
         */
        void setPosition(const Hex &newPosition);

        /**************************************************************************************************
         * Operators
         *************************************************************************************************/

        /**
         * @brief Stream insertion operator for debugging and logging.
         * @param os The output stream.
         * @param piece The Piece to output.
         * @return The output stream.
         */
        friend std::ostream &operator<<(std::ostream &os, const Piece &piece);

        /**
         * @brief Equality operator for comparing two pieces.
         * @param other The other piece to compare with.
         * @return True if both pieces have the same ID, type and owner, false otherwise.
         */
        bool operator==(const Piece &other) const;

        /**
         * @brief Inequality operator for comparing two pieces.
         * @param other The other piece to compare with.
         * @return True if the pieces differ in ID, false otherwise.
         */
        bool operator!=(const Piece &other) const { return !(*this == other); }

        /**************************************************************************************************
         * Public methods
         *************************************************************************************************/

        /**
         * @brief Retrieves the neighboring pieces of this piece on the board.
         * @param board A reference to the board object to get neighboring hexes and pieces.
         * @return A vector of shared pointers to the neighboring pieces (read-only).
         *
         * This method ensures that the returned pieces are valid (non-null) and cannot
         * be modified through this method.
         */
        [[nodiscard]] std::vector<std::shared_ptr<const Piece> > getNeighbors(const Board &board) const;

        /**
         * @brief Checks if the piece has an owner.
         * @return True if the piece has an owner, false otherwise.
         */
        [[nodiscard]] bool hasOwner() const { return !owner.expired(); }

        /**
         * @brief Resets the position of the piece to an empty optional.
         */
        void resetPosition() { position.reset(); }

        /**
        * @brief Converts the Piece object to a string representation.
        * @return A string representation of the Piece object.
        */
        [[nodiscard]] std::string toString() const;
    };
} // namespace hive::models

#endif // PIECE_H

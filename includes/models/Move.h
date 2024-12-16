#ifndef MOVE_H
#define MOVE_H

#include <memory>
#include "models/Hex.h"
#include "models/Piece.h"
#include "models/Player.h"
#include "models/Board.h"

namespace hive::models {
    /**
     * @class Move
     * @brief Represents a move in the Hive game.
     *
     * The `Move` class encapsulates the data required to execute and undo a move.
     * A move can either be placing a new piece or moving an existing piece on the board.
     * Objects of this class are immutable once created.
     */
    class Move {
    public:
        /**
         * @enum MoveType
         * @brief Represents the type of move.
         *
         * - PLACE: Indicates the placement of a new piece on the board.
         * - MOVE: Indicates the movement of an existing piece from one hex to another.
         */
        enum class MoveType {
            PLACE, /**< Represents a move where a piece is placed. */
            MOVE /**< Represents a move where a piece is moved. */
        };

    private:
        const MoveType type; /**< The type of move (PLACE or MOVE). */
        const std::weak_ptr<Player> player; /**< Weak reference to the player making the move. */
        const std::weak_ptr<Piece> piece; /**< Weak reference to the piece being placed or moved. */
        const std::optional<Hex> from; /**< The starting position (used for MOVE type only). */
        const Hex to; /**< The target position. */

    public:
        /**************************************************************************************************
         * Constructors
         *************************************************************************************************/

        /**
         * @brief Constructs a placement move.
         * @param player Shared pointer to the player making the move (must not be null).
         * @param piece Shared pointer to the piece to place (must not be null).
         * @param to The target position for placing the piece.
         *
         * @throws std::invalid_argument If `player` or `piece` is null.
         */
        Move(const std::shared_ptr<Player> &player,
             const std::shared_ptr<Piece> &piece,
             Hex to);

        /**
         * @brief Constructs a movement move.
         * @param player Shared pointer to the player making the move (must not be null).
         * @param piece Shared pointer to the piece to move (must not be null).
         * @param from The starting position of the piece.
         * @param to The target position of the piece.
         *
         * @throws std::invalid_argument If `player` or `piece` is null.
         */
        Move(const std::shared_ptr<Player> &player,
             const std::shared_ptr<Piece> &piece,
             const Hex &from,
             Hex to);

        /**************************************************************************************************
         * Getters
         *************************************************************************************************/

        /**
         * @brief Retrieves the player who made the move.
         * @return A shared pointer to the player.
         */
        [[nodiscard]] std::shared_ptr<Player> getPlayer() const;

        /**
         * @brief Retrieves the type of the move.
         * @return The type of the move (either PLACE or MOVE).
         */
        [[nodiscard]] MoveType getType() const { return type; }

        /**
         * @brief Retrieves the piece involved in the move.
         * @return A shared pointer to the piece.
         */
        [[nodiscard]] std::shared_ptr<Piece> getPiece() const;

        /**
         * @brief Retrieves the starting position of the piece.
         * @return The starting position (optional).
         *
         * This is valid only for MOVE type moves. For PLACE moves, this will return `std::nullopt`.
         */
        [[nodiscard]] std::optional<Hex> getFrom() const { return from; }

        /**
         * @brief Retrieves the target position of the piece.
         * @return The target position.
         */
        [[nodiscard]] const Hex &getTo() const { return to; }

        /**************************************************************************************************
         * Public Methods
         *************************************************************************************************/

        /**
         * @brief Executes the move on the specified game board.
         * @param board The game board on which to execute the move.
         *
         * Executes the move by placing the piece on the target hex for a PLACE move,
         * or moving the piece from the starting position to the target position for a MOVE move.
         * @throws std::runtime_error If the move type is invalid.
         */
        void execute(Board &board) const;

        /**
         * @brief Undoes the move on the specified game board.
         * @param board The game board on which to undo the move.
         *
         * Reverts the move by removing the piece from the target hex for a PLACE move,
         * or moving the piece back to the starting position for a MOVE move.
         * @throws std::runtime_error If the move type is invalid.
         */
        void undo(Board &board) const;

        /**
         * @brief Converts the Move object to a string representation.
         * @return A string representation of the Move object.
         */
        [[nodiscard]] std::string toString() const;

        /**************************************************************************************************
         * Operators
         *************************************************************************************************/

        /**
         * @brief Overloads the stream insertion operator for the Move class.
         * @param os The output stream.
         * @param move The Move object to output.
         * @return A reference to the output stream.
         */
        friend std::ostream &operator<<(std::ostream &os, const Move &move);
    };
} // namespace hive::models

#endif // MOVE_H

#ifndef MOVE_H
#define MOVE_H

#include <memory>
#include "models/Hex.h"
#include "models/Piece.h"
#include "models/Player.h"  // Inclusion de la classe Player

namespace hive::models {
    /**
     * @class Move
     * @brief Represents a move in the Hive game.
     *
     * The Move class encapsulates the data required to execute and undo a move.
     * A move can either be placing a new piece or moving an existing piece on the board.
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
        MoveType type; /**< The type of move (PLACE or MOVE). */
        Player *player; /**< Pointer to the player making the move. */
        Piece *piece; /**< Pointer to the piece being placed or moved. */
        Hex from; /**< The starting position (used for MOVE type only). */
        Hex to; /**< The target position. */

    public:
        /**
         * @brief Constructs a placement move.
         * @param player The player making the move (must not be null).
         * @param piece The piece to place (must not be null).
         * @param to The target position for placing the piece.
         *
         * Initializes a move of type PLACE, where a new piece is placed on the board
         * at the specified position.
         * @throws std::invalid_argument If the piece is null.
         */
        Move(Player *player, Piece *piece, const Hex &to);

        /**
         * @brief Constructs a movement move.
         * @param player The player making the move (must not be null).
         * @param piece The piece to move (must not be null).
         * @param from The starting position of the piece.
         * @param to The target position of the piece.
         *
         * Initializes a move of type MOVE, where an existing piece is moved from a
         * starting position to a target position.
         * @throws std::invalid_argument If the piece is null.
         */
        Move(Player *player, Piece *piece, const Hex &from, const Hex &to);

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
         * @brief Retrieves the player who made the move.
         * @return A pointer to the player.
         */
        [[nodiscard]] const Player *getPlayer() const { return player; }

        /**
         * @brief Retrieves the type of the move.
         * @return The type of the move (either PLACE or MOVE).
         */
        [[nodiscard]] MoveType getType() const { return type; }

        /**
         * @brief Retrieves the piece involved in the move.
         * @return A pointer to the piece.
         */
        [[nodiscard]] const Piece *getPiece() const { return piece; }

        /**
         * @brief Retrieves the starting position of the piece.
         * @return The starting position.
         *
         * This method is only valid for MOVE type moves.
         * @throws std::runtime_error If called for a PLACE move.
         */
        [[nodiscard]] const Hex &getFrom() const;

        /**
         * @brief Retrieves the target position of the piece.
         * @return The target position.
         */
        [[nodiscard]] const Hex &getTo() const { return to; }

        /**
         * @brief Destructor.
         *
         * Ensures no resource leaks and safe cleanup of the Move instance.
         */
        ~Move() = default;
    };
} // namespace hive::models

#endif // MOVE_H

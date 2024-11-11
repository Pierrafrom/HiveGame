#ifndef MOVE_H
#define MOVE_H

#include <memory>
#include "models/Hex.h"
#include "models/Piece.h"

//TODO: Create a test suite for the Move class
//TODO: Implement the Move class

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
            PLACE,
            MOVE
        };

    private:
        MoveType type; /**< The type of move (PLACE or MOVE) */
        size_t playerId; /**< ID of the player making the move */
        std::unique_ptr<Piece> piece; /**< The piece being placed or moved */
        Hex from; /**< The starting position (used for MOVE type only) */
        Hex to; /**< The target position */

    public:
        /**
         * @brief Constructs a placement move.
         * @param playerId The ID of the player making the move.
         * @param piece The piece to place.
         * @param to The target position for placing the piece.
         *
         * Initializes a move of type PLACE, where a new piece is placed on the board at the specified position.
         */
        Move(size_t playerId, std::unique_ptr<Piece> piece, const Hex &to);

        /**
         * @brief Constructs a movement move.
         * @param playerId The ID of the player making the move.
         * @param piece The piece to move.
         * @param from The starting position of the piece.
         * @param to The target position of the piece.
         *
         * Initializes a move of type MOVE, where an existing piece is moved from a starting position to a target position.
         */
        Move(size_t playerId, Piece *piece, const Hex &from, const Hex &to);

        /**
         * @brief Executes the move on the specified game board.
         * @param board The game board on which to execute the move.
         *
         * Places the piece on the target hex for a PLACE move or moves the piece from the starting position to the target position for a MOVE move.
         */
        void execute(Board &board);

        /**
         * @brief Undoes the move on the specified game board.
         * @param board The game board on which to undo the move.
         *
         * Reverts the move by removing the piece from the target hex for a PLACE move or returning the piece to its starting position for a MOVE move.
         */
        void undo(Board &board);

        /**
         * @brief Retrieves the ID of the player who made the move.
         * @return The ID of the player.
         */
        [[nodiscard]] size_t getPlayerId() const;

        /**
         * @brief Retrieves the type of the move.
         * @return The type of the move (either PLACE or MOVE).
         */
        [[nodiscard]] MoveType getType() const;

        /**
         * @brief Destructor.
         *
         * Defaulted destructor, as no special cleanup is required.
         */
        ~Move() = default;
    };
} // namespace hive::models

#endif // MOVE_H

#ifndef MOVE_STRATEGY_H
#define MOVE_STRATEGY_H

#include <vector>
#include "models/Hex.h"

// Forward declaration of Board class
namespace hive::models {
    class Piece;
    class Board;
    class Player;
}

namespace hive::models::strategies {
    /**
     * @class MoveStrategy
     * @brief Abstract class defining the movement strategy for game pieces.
     *
     * This class provides an interface to calculate the possible moves for a piece
     * based on its current position and the state of the game board.
     */
    class MoveStrategy {
    protected:
        Piece *piece;

        /**
        * @brief Checks if a hive is connected after moving a piece.
        * @param board A copy of the game board.
        * @param from The original position of the piece.
        * @param to The destination position of the piece.
        * @return True if the hive remains connected after moving the piece, false otherwise.
        */
        static bool isHiveConnectedAfterMove(Board board, Hex from, Hex to);

    public:
        /**************************************************************************************************
         * Constructors
         *************************************************************************************************/

        /**
         * @brief Constructs a MoveStrategy object for a specific piece.
         * @param p Pointer to the piece that uses this movement strategy.
         */
        explicit MoveStrategy(Piece *p) : piece(p) {
        }

        /**************************************************************************************************
         * Destructor
         *************************************************************************************************/
        /**
         * @brief Virtual destructor.
         *
         * Ensures that derived classes can override the destructor and clean up any resources if necessary.
         */
        virtual ~MoveStrategy() = default;

        /**************************************************************************************************
         * Public methods
         *************************************************************************************************/

        /**
         * @brief Calculates the possible moves for a piece from a given position.
         * @param board The current state of the game board.
         * @param player The player that owns the piece.
         * @return A vector of `Hex` objects representing reachable positions.
         *
         * This pure virtual method must be overridden by derived classes to provide
         * specific movement logic for each type of piece.
         */
        [[nodiscard]] virtual std::vector<Hex> getPossibleMoves(const Board &board, const Player &player) const = 0;

        /**
        * @brief Checks if the piece has a valid position.
        * @return True if the piece has a valid position, false otherwise.
        */
        [[nodiscard]] bool hasPiece() const { return piece != nullptr; }

        /**
        * @brief Retrieves the piece that uses this movement strategy.
        * @return A reference to the piece object.
        */
        [[nodiscard]] const Piece &getPiece() const;

        /**
        * @brief Validates the ownership of the piece by the player.
        * @param player The player to validate ownership against.
        * @return True if the player owns the piece, false otherwise.
        */
        [[nodiscard]] bool validatePieceOwnership(const Player &player) const;
    };
} // namespace hive::models::strategies

#endif // MOVE_STRATEGY_H

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
         * @param position The current position of the piece on the board.
         * @param board The current state of the game board.
         * @param player The player that owns the piece.
         * @return A vector of `Hex` objects representing reachable positions.
         *
         * This pure virtual method must be overridden by derived classes to provide
         * specific movement logic for each type of piece.
         */
        [[nodiscard]] virtual std::vector<Hex> getPossibleMoves(const Hex &position, const Board &board,
                                                                const Player &player) const = 0;

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
    };
} // namespace hive::models::strategies

#endif // MOVE_STRATEGY_H

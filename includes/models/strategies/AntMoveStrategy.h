#ifndef ANT_MOVE_STRATEGY_H
#define ANT_MOVE_STRATEGY_H

#include <models/Player.h>

#include "models/strategies/MoveStrategy.h"

namespace hive::models::strategies {
    /**
     * @class AntMoveStrategy
     * @brief Movement strategy for the Ant piece in the Hive game.
     *
     * This class implements the specific movement rules for the Ant piece, allowing it to traverse
     * multiple spaces on the board, provided it stays adjacent to other pieces.
     */
    class AntMoveStrategy final : public MoveStrategy {
    public:
        /**
         * @brief Constructs a MoveStrategy object for a specific piece.
         * @param p Pointer to the piece that uses this movement strategy.
         */
        explicit AntMoveStrategy(Piece *p) : MoveStrategy(p) {
        }

        /**
         * @brief Destructor.
         *
         * Ensures proper cleanup for the Ant's movement strategy if necessary.
         */
        ~AntMoveStrategy() override = default;

        /**
         * @brief Calculates the possible moves for the Ant piece.
         * @param board The current state of the game board.
         * @param player The player that owns the Ant.
         * @return A vector of `Hex` objects representing all reachable positions.
         *
         * This method overrides `getPossibleMoves` to provide the unique movement behavior for the Ant.
         * The Ant can move to any location on the perimeter of connected pieces, following
         * the sliding rule that keeps it adjacent to other pieces.
         */
        [[nodiscard]] std::vector<Hex> getPossibleMoves(const Board &board, const Player &player) const override;

        std::vector<Hex> addPossibleMove(const Hex &current_position, std::vector<Hex> &possibleMoves, const Board &board, const Hex &initiaPosition) const;

        void antExploration(const Hex &current_position, std::vector<Hex> &possibleMoves, const Board &board, const Hex &initiaPosition) const;

    };
} // namespace hive::models::strategies

#endif // ANT_MOVE_STRATEGY_H

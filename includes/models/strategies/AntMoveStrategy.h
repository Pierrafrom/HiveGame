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
         * @brief Destructor.
         *
         * Ensures proper cleanup for the Ant's movement strategy if necessary.
         */
        ~AntMoveStrategy() override = default;

        /**
         * @brief Calculates the possible moves for the Ant piece.
         * @param position The current position of the Ant on the board.
         * @param board The current state of the game board.
         * @param player The player that owns the Ant.
         * @return A vector of `Hex` objects representing all reachable positions.
         *
         * This method overrides `getPossibleMoves` to provide the unique movement behavior for the Ant.
         * The Ant can move to any location on the perimeter of connected pieces, following
         * the sliding rule that keeps it adjacent to other pieces.
         */
        [[nodiscard]] std::vector<Hex> getPossibleMoves(const Hex &position,
                                                        const Board &board,
                                                        const Player &player) const override;
    };
} // namespace hive::models::strategies

#endif // ANT_MOVE_STRATEGY_H

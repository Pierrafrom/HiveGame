#ifndef QUEEN_BEE_MOVE_STRATEGY_H
#define QUEEN_BEE_MOVE_STRATEGY_H

#include <models/Player.h>

#include "models/strategies/MoveStrategy.h"

namespace hive::models::strategies {
    /**
     * @class QueenBeeMoveStrategy
     * @brief Movement strategy for the Queen Bee piece in the Hive game.
     *
     * Implements the specific movement rules for the Queen Bee, allowing it to move only one space at a time.
     */
    class QueenBeeMoveStrategy final : public MoveStrategy {
    public:
        /**
         * @brief Destructor.
         */
        ~QueenBeeMoveStrategy() override = default;

        /**
         * @brief Calculates the possible moves for the Queen Bee piece.
         * @param position The current position of the Queen Bee on the board.
         * @param board The current state of the game board.
         * @param player The player that owns the Queen Bee.
         * @return A vector of `Hex` objects representing all reachable positions.
         *
         * The Queen Bee can move only one space at a time to adjacent cells.
         */
        [[nodiscard]] std::vector<Hex> getPossibleMoves(const Hex &position,
                                                        const Board &board,
                                                        const Player &player) const override;
    };
} // namespace hive::models::strategies

#endif // QUEEN_BEE_MOVE_STRATEGY_H

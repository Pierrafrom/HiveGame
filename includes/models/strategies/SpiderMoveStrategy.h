#ifndef SPIDER_MOVE_STRATEGY_H
#define SPIDER_MOVE_STRATEGY_H

#include "models/strategies/MoveStrategy.h"

namespace hive::models::strategies {
    /**
     * @class SpiderMoveStrategy
     * @brief Movement strategy for the Spider piece in the Hive game.
     *
     * Implements the specific movement rules for the Spider, which must move exactly three spaces.
     */
    class SpiderMoveStrategy final : public MoveStrategy {
    public:
        /**
         * @brief Destructor.
         */
        ~SpiderMoveStrategy() override = default;

        /**
         * @brief Calculates the possible moves for the Spider piece.
         * @param position The current position of the Spider on the board.
         * @param board The current state of the game board.
         * @param player The player that owns the Spider.
         * @return A vector of `Hex` objects representing all reachable positions.
         *
         * The Spider must move exactly three spaces, following the sliding rule.
         */
        [[nodiscard]] std::vector<Hex> getPossibleMoves(const Hex &position,
                                                        const Board &board,
                                                        const Player &player) const override;
    };
} // namespace hive::models::strategies

#endif // SPIDER_MOVE_STRATEGY_H

#ifndef SPIDER_MOVE_STRATEGY_H
#define SPIDER_MOVE_STRATEGY_H

#include "models/strategies/MoveStrategy.h"
#include <set>

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
         * @brief Constructs a MoveStrategy object for a specific piece.
         * @param p Pointer to the piece that uses this movement strategy.
         */
        explicit SpiderMoveStrategy(Piece *p) : MoveStrategy(p) {
        }

        /**
         * @brief Destructor.
         */
        ~SpiderMoveStrategy() override = default;

        /**
         * @brief Calculates the possible moves for the Spider piece.
         * @param board The current state of the game board.
         * @param player The player that owns the Spider.
         * @return A vector of `Hex` objects representing all reachable positions.
         *
         * The Spider must move exactly three spaces, following the sliding rule.
         */
        [[nodiscard]] std::vector<Hex> getPossibleMoves(const Board &board, const Player &player) const override;

         /**
        * @brief Recursively explores the reachable positions for the Spider.
        * @param current The current position being explored.
        * @param
        */
        void spiderExploration(const Hex &current, int depth, std::set<Hex> &visited, std::set<Hex> &reachable, const Board &board) const;
    };
} // namespace hive::models::strategies

#endif // SPIDER_MOVE_STRATEGY_H

#ifndef BEETLE_MOVE_STRATEGY_H
#define BEETLE_MOVE_STRATEGY_H

#include "models/strategies/MoveStrategy.h"

namespace hive::models::strategies {
    /**
     * @class BeetleMoveStrategy
     * @brief Movement strategy for the Beetle piece in the Hive game.
     *
     * Implements the specific movement rules for the Beetle piece, allowing it to move one space at a time
     * and also climb on top of other pieces.
     */
    class BeetleMoveStrategy final : public MoveStrategy {
    public:
        /**
         * @brief Destructor.
         */
        ~BeetleMoveStrategy() override = default;

        /**
         * @brief Calculates the possible moves for the Beetle piece.
         * @param position The current position of the Beetle on the board.
         * @param board The current state of the game board.
         * @return A vector of `Hex` objects representing all reachable positions.
         *
         * The Beetle can move one space at a time and can also move on top of other pieces,
         * allowing it to climb and block them.
         */
        [[nodiscard]] std::vector<Hex> getPossibleMoves(const Hex &position,
                                                        const Board &board) const override;
    };
} // namespace hive::models::strategies

#endif // BEETLE_MOVE_STRATEGY_H

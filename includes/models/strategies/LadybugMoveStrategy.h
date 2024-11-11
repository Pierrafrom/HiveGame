#ifndef LADYBUG_MOVE_STRATEGY_H
#define LADYBUG_MOVE_STRATEGY_H

#include "models/strategies/MoveStrategy.h"

namespace hive::models::strategies {
    /**
     * @class LadybugMoveStrategy
     * @brief Movement strategy for the Ladybug piece in the Hive game.
     *
     * The Ladybug has a unique movement rule allowing it to move exactly three spaces:
     * it must move two spaces on top of other pieces, then descend to an open space.
     */
    class LadybugMoveStrategy final : public MoveStrategy {
    public:
        /**
         * @brief Destructor.
         *
         * Ensures proper cleanup for the Ladybug's movement strategy if necessary.
         */
        ~LadybugMoveStrategy() override = default;

        /**
         * @brief Calculates the possible moves for the Ladybug piece.
         * @param position The current position of the Ladybug on the board.
         * @param board The current state of the game board.
         * @return A vector of `Hex` objects representing all reachable positions.
         *
         * The Ladybug moves exactly three spaces: the first two must be on top of other pieces,
         * and the final space must be an open space adjacent to the hive.
         */
        [[nodiscard]] std::vector<Hex> getPossibleMoves(const Hex &position,
                                                        const Board &board) const override;
    };
} // namespace hive::models::strategies

#endif // LADYBUG_MOVE_STRATEGY_H

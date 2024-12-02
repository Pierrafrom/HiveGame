#ifndef PILLBUG_MOVE_STRATEGY_H
#define PILLBUG_MOVE_STRATEGY_H

#include <models/Player.h>

#include "models/strategies/MoveStrategy.h"

namespace hive::models::strategies {
    /**
     * @class PillbugMoveStrategy
     * @brief Movement strategy for the Pillbug piece in the Hive game.
     *
     * This class implements the unique movement and special ability rules for the Pillbug piece.
     * The Pillbug can move one space like other pieces, but it also has the ability to lift and move adjacent pieces.
     */
    class PillbugMoveStrategy final : public MoveStrategy {
    public:
        /**
         * @brief Destructor.
         *
         * Ensures proper cleanup for the Pillbug's movement strategy if necessary.
         */
        ~PillbugMoveStrategy() override = default;

        /**
         * @brief Calculates the possible moves for the Pillbug piece.
         * @param position The current position of the Pillbug on the board.
         * @param board The current state of the game board.
         * @param player The player that owns the Pillbug.
         * @return A vector of `Hex` objects representing all reachable positions.
         *
         * The Pillbug can move one space at a time, but it also has a special ability to lift and move adjacent pieces,
         * subject to specific game rules.
         */
        [[nodiscard]] std::vector<Hex> getPossibleMoves(const Hex &position,
                                                        const Board &board,
                                                        const Player &player) const override;
    };
} // namespace hive::models::strategies

#endif // PILLBUG_MOVE_STRATEGY_H

#ifndef GRASSHOPPER_MOVE_STRATEGY_H
#define GRASSHOPPER_MOVE_STRATEGY_H

#include <models/Player.h>

#include "models/strategies/MoveStrategy.h"

namespace hive::models::strategies {
    /**
     * @class GrasshopperMoveStrategy
     * @brief Movement strategy for the Grasshopper piece in the Hive game.
     *
     * Implements the specific movement rules for the Grasshopper piece, which allows it to jump over other pieces.
     */
    class GrasshopperMoveStrategy final : public MoveStrategy {
    public:
        /**
         * @brief Constructs a MoveStrategy object for a specific piece.
         * @param p Pointer to the piece that uses this movement strategy.
         */
        explicit GrasshopperMoveStrategy(Piece *p) : MoveStrategy(p) {
        }

        /**
         * @brief Destructor.
         */
        ~GrasshopperMoveStrategy() override = default;

        /**
         * @brief Calculates the possible moves for the Grasshopper piece.
         * @param position The current position of the Grasshopper on the board.
         * @param board The current state of the game board.
         * @param player The player that owns the Grasshopper.
         * @return A vector of `Hex` objects representing all reachable positions.
         *
         * The Grasshopper can jump in a straight line over adjacent pieces, landing on the first empty space beyond them.
         */
        [[nodiscard]] std::vector<Hex> getPossibleMoves(const Hex &position,
                                                        const Board &board,
                                                        const Player &player) const override;
    };
} // namespace hive::models::strategies

#endif // GRASSHOPPER_MOVE_STRATEGY_H

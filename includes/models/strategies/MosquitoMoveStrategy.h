#ifndef MOSQUITO_MOVE_STRATEGY_H
#define MOSQUITO_MOVE_STRATEGY_H

#include <models/Player.h>

#include "models/strategies/MoveStrategy.h"

namespace hive::models::strategies {
    /**
     * @class MosquitoMoveStrategy
     * @brief Movement strategy for the Mosquito piece in the Hive game.
     *
     * The Mosquito imitates the movement abilities of adjacent pieces, making its movement
     * behavior dynamic based on the current game state.
     */
    class MosquitoMoveStrategy final : public MoveStrategy {
    public:
        /**
         * @brief Constructs a MoveStrategy object for a specific piece.
         * @param p Pointer to the piece that uses this movement strategy.
         */
        explicit MosquitoMoveStrategy(Piece *p) : MoveStrategy(p) {
        }

        /**
         * @brief Destructor.
         *
         * Ensures proper cleanup for the Mosquito's movement strategy if necessary.
         */
        ~MosquitoMoveStrategy() override = default;

        /**
         * @brief Calculates the possible moves for the Mosquito piece.
         * @param position The current position of the Mosquito on the board.
         * @param board The current state of the game board.
         * @param player The player that owns the Mosquito.
         * @return A vector of `Hex` objects representing all reachable positions.
         *
         * The Mosquito can move according to the movement abilities of any adjacent piece.
         */
        [[nodiscard]] std::vector<Hex> getPossibleMoves(const Hex &position,
                                                        const Board &board,
                                                        const Player &player) const override;
    };
} // namespace hive::models::strategies

#endif // MOSQUITO_MOVE_STRATEGY_H

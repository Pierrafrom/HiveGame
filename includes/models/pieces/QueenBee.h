#ifndef QUEENBEE_H
#define QUEENBEE_H

#include "models/Piece.h"
#include "models/strategies/QueenBeeMoveStrategy.h"

namespace hive::models::pieces {
    /**
     * @class QueenBee
     * @brief Represents a Queen Bee piece in the Hive game.
     *
     * The `QueenBee` class inherits from the abstract `Piece` class and encapsulates
     * the unique characteristics and movement behavior of the Queen Bee piece in Hive.
     * The Queen Bee has specific movement rules that allow it to move only one space
     * at a time.
     */
    class QueenBee final : public Piece {
    public:
        /**
         * @brief Constructor for the QueenBee piece with an optional specified ID.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         *
         * This constructor initializes a QueenBee piece, assigning a `QueenBeeMoveStrategy` for its unique movement.
         */
        explicit QueenBee(const size_t id = pieceNextId++) : Piece(id, enums::PieceType::QUEEN_BEE,
                                                              std::make_unique<strategies::QueenBeeMoveStrategy>()) {
        }

        /**
         * @brief Destructor for the QueenBee piece.
         *
         * The destructor is defaulted as no special cleanup is necessary.
         */
        ~QueenBee() override = default;
    };
} // namespace hive::models::pieces

#endif // QUEENBEE_H

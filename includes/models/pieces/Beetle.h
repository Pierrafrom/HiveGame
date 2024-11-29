#ifndef BEETLE_H
#define BEETLE_H

#include "models/Piece.h"
#include "models/strategies/BeetleMoveStrategy.h"

namespace hive::models::pieces {
    /**
     * @class Beetle
     * @brief Represents a Beetle piece in the Hive game.
     *
     * The `Beetle` class inherits from the abstract `Piece` class and encapsulates
     * the unique characteristics and movement behavior of the Beetle piece in Hive.
     * The Beetle can climb on top of other pieces, affecting the hive structure.
     */
    class Beetle final : public Piece {
    public:
        /**
         * @brief Constructor for the Beetle piece with an optional specified ID.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         *
         * This constructor initializes a Beetle piece, assigning a `BeetleMoveStrategy` for its unique movement.
         */
        explicit Beetle(const size_t id = pieceNextId++) : Piece(id, enums::PieceType::BEETLE,
                                                            std::make_unique<strategies::BeetleMoveStrategy>()) {
        }

        /**
         * @brief Destructor for the Beetle piece.
         *
         * The destructor is defaulted as no special cleanup is necessary.
         */
        ~Beetle() override = default;
    };
} // namespace hive::models::pieces

#endif // BEETLE_H

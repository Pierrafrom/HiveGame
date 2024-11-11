#ifndef LADYBUG_H
#define LADYBUG_H

#include "models/Piece.h"
#include "models/strategies/LadybugMoveStrategy.h"

namespace hive::models::pieces {
    /**
     * @class Ladybug
     * @brief Represents a Ladybug piece in the Hive game.
     *
     * The `Ladybug` class inherits from the abstract `Piece` class and encapsulates
     * the unique characteristics and movement behavior of the Ladybug piece in Hive.
     * The Ladybug moves exactly three spaces, climbing over the hive before descending.
     */
    class Ladybug final : public Piece {
    public:
        /**
         * @brief Constructor for the Ladybug piece with an optional specified ID.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         *
         * This constructor initializes a Ladybug piece, assigning a `LadybugMoveStrategy` for its unique movement.
         */
        explicit Ladybug(size_t id = nextId++) : Piece(id, enums::PieceType::LADYBUG,
                                                       std::make_unique<strategies::LadybugMoveStrategy>()) {
        }

        /**
         * @brief Destructor for the Ladybug piece.
         *
         * The destructor is defaulted as no special cleanup is necessary.
         */
        ~Ladybug() override = default;
    };
} // namespace hive::models::pieces

#endif // LADYBUG_H

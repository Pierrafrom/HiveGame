#ifndef PILLBUG_H
#define PILLBUG_H

#include "models/Piece.h"
#include "models/strategies/PillbugMoveStrategy.h"

namespace hive::models::pieces {
    /**
     * @class Pillbug
     * @brief Represents a Pillbug piece in the Hive game.
     *
     * The `Pillbug` class inherits from the abstract `Piece` class and encapsulates
     * the unique movement and special ability of the Pillbug piece in Hive.
     * The Pillbug can move like a regular piece and also has the ability to pull
     * adjacent pieces toward itself.
     */
    class Pillbug final : public Piece {
    public:
        /**
         * @brief Constructor for the Pillbug piece with an optional specified ID.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         *
         * This constructor initializes a Pillbug piece, assigning a `PillbugMoveStrategy` for its movement
         * and special pulling ability.
         */
        explicit Pillbug(const size_t id = nextId++) : Piece(id, enums::PieceType::PILLBUG,
                                                             std::make_unique<strategies::PillbugMoveStrategy>()) {
        }

        /**
         * @brief Destructor for the Pillbug piece.
         *
         * The destructor is defaulted as no special cleanup is necessary.
         */
        ~Pillbug() override = default;
    };
} // namespace hive::models::pieces

#endif // PILLBUG_H

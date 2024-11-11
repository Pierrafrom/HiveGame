#ifndef GRASSHOPPER_H
#define GRASSHOPPER_H

#include "models/Piece.h"
#include "models/strategies/GrasshopperMoveStrategy.h"

namespace hive::models::pieces {
    /**
     * @class Grasshopper
     * @brief Represents a Grasshopper piece in the Hive game.
     *
     * The `Grasshopper` class inherits from the abstract `Piece` class and encapsulates
     * the unique characteristics and movement behavior of the Grasshopper piece in Hive.
     * The Grasshopper can jump over a line of pieces to land on the opposite side.
     */
    class Grasshopper final : public Piece {
    public:
        /**
         * @brief Constructor for the Grasshopper piece with an optional specified ID.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         *
         * This constructor initializes a Grasshopper piece, assigning a `GrasshopperMoveStrategy` for its unique movement.
         */
        explicit Grasshopper(const size_t id = nextId++) : Piece(id, enums::PieceType::GRASSHOPPER,
                                                                 std::make_unique<
                                                                     strategies::GrasshopperMoveStrategy>()) {
        }

        /**
         * @brief Destructor for the Grasshopper piece.
         *
         * The destructor is defaulted as no special cleanup is necessary.
         */
        ~Grasshopper() override = default;
    };
} // namespace hive::models::pieces

#endif // GRASSHOPPER_H

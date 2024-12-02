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
        /**************************************************************************************************
         * Constructors
         *************************************************************************************************/

        /**
         * @brief Constructs a Grasshopper piece with an optional specified ID, owner, and position.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         * @param owner Shared pointer to the owning player (default is nullptr).
         * @param position Optional initial position of the piece on the board (default is std::nullopt).
         *
         * This constructor initializes a Grasshopper piece, assigning a `GrasshopperMoveStrategy` for its unique movement.
         */
        explicit Grasshopper(const size_t id = pieceNextId++,
                             const std::shared_ptr<Player> &owner = nullptr,
                             const std::optional<Hex> &position = std::nullopt)
            : Piece(id,
                    enums::PieceType::GRASSHOPPER,
                    std::make_unique<strategies::GrasshopperMoveStrategy>(),
                    owner,
                    position) {
        }

        /**************************************************************************************************
         * Destructor
         *************************************************************************************************/

        /**
         * @brief Default destructor for the Grasshopper piece.
         *
         * Ensures proper cleanup of resources specific to the Grasshopper piece. Declared as `override`
         * to enforce polymorphic deletion for derived classes.
         */
        ~Grasshopper() override = default;

        /**************************************************************************************************
         * Other constructors and operators
         *************************************************************************************************/

        /**
         * @brief Delete copy constructor.
         */
        Grasshopper(const Grasshopper &) = delete;

        /**
         * @brief Delete copy assignment operator.
         */
        Grasshopper &operator=(const Grasshopper &) = delete;

        /**
         * @brief Default move constructor.
         */
        Grasshopper(Grasshopper &&) noexcept = default;

        /**
         * @brief Default move assignment operator.
         */
        Grasshopper &operator=(Grasshopper &&) noexcept = default;
    };
} // namespace hive::models::pieces

#endif // GRASSHOPPER_H

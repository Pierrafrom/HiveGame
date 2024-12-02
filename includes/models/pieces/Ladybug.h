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
        /**************************************************************************************************
         * Constructors
         *************************************************************************************************/

        /**
         * @brief Constructs a Ladybug piece with an optional specified ID, owner, and position.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         * @param owner Shared pointer to the owning player (default is nullptr).
         * @param position Optional initial position of the piece on the board (default is std::nullopt).
         *
         * This constructor initializes a Ladybug piece, assigning a `LadybugMoveStrategy` for its unique movement.
         */
        explicit Ladybug(const size_t id = pieceNextId++,
                         const std::shared_ptr<Player> &owner = nullptr,
                         const std::optional<Hex> &position = std::nullopt)
            : Piece(id,
                    enums::PieceType::LADYBUG,
                    std::make_unique<strategies::LadybugMoveStrategy>(),
                    owner,
                    position) {
        }

        /**************************************************************************************************
         * Destructor
         *************************************************************************************************/

        /**
         * @brief Default destructor for the Ladybug piece.
         *
         * Ensures proper cleanup of resources specific to the Ladybug piece. Declared as `override`
         * to enforce polymorphic deletion for derived classes.
         */
        ~Ladybug() override = default;

        /**************************************************************************************************
         * Other constructors and operators
         *************************************************************************************************/

        /**
         * @brief Delete copy constructor.
         */
        Ladybug(const Ladybug &) = delete;

        /**
         * @brief Delete copy assignment operator.
         */
        Ladybug &operator=(const Ladybug &) = delete;

        /**
         * @brief Default move constructor.
         */
        Ladybug(Ladybug &&) noexcept = default;

        /**
         * @brief Default move assignment operator.
         */
        Ladybug &operator=(Ladybug &&) noexcept = default;
    };
} // namespace hive::models::pieces

#endif // LADYBUG_H

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
        /**************************************************************************************************
         * Constructors
         *************************************************************************************************/

        /**
         * @brief Constructs a Pillbug piece with an optional specified ID, owner, and position.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         * @param owner Shared pointer to the owning player (default is nullptr).
         * @param position Optional initial position of the piece on the board (default is std::nullopt).
         *
         * This constructor initializes a Pillbug piece, assigning a `PillbugMoveStrategy` for its movement
         * and special pulling ability.
         */
        explicit Pillbug(const size_t id = pieceNextId++,
                         const std::shared_ptr<Player> &owner = nullptr,
                         const std::optional<Hex> &position = std::nullopt)
            : Piece(id,
                    enums::PieceType::PILLBUG,
                    std::make_unique<strategies::PillbugMoveStrategy>(),
                    owner,
                    position) {
        }

        /**************************************************************************************************
         * Destructor
         *************************************************************************************************/

        /**
         * @brief Default destructor for the Pillbug piece.
         *
         * Ensures proper cleanup of resources specific to the Pillbug piece. Declared as `override`
         * to enforce polymorphic deletion for derived classes.
         */
        ~Pillbug() override = default;

        /**************************************************************************************************
         * Other constructors and operators
         *************************************************************************************************/

        /**
         * @brief Delete copy constructor.
         */
        Pillbug(const Pillbug &) = delete;

        /**
         * @brief Delete copy assignment operator.
         */
        Pillbug &operator=(const Pillbug &) = delete;

        /**
         * @brief Default move constructor.
         */
        Pillbug(Pillbug &&) noexcept = default;

        /**
         * @brief Default move assignment operator.
         */
        Pillbug &operator=(Pillbug &&) noexcept = default;
    };
} // namespace hive::models::pieces

#endif // PILLBUG_H

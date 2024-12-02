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
        /**************************************************************************************************
         * Constructors
         *************************************************************************************************/

        /**
         * @brief Constructs a Beetle piece with an optional specified ID, owner, and position.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         * @param owner Shared pointer to the owning player (default is nullptr).
         * @param position Optional initial position of the piece on the board (default is std::nullopt).
         *
         * This constructor initializes a Beetle piece, assigning a `BeetleMoveStrategy` for its unique movement.
         */
        explicit Beetle(const size_t id = pieceNextId++,
                        const std::shared_ptr<Player> &owner = nullptr,
                        const std::optional<Hex> &position = std::nullopt)
            : Piece(id,
                    enums::PieceType::BEETLE,
                    std::make_unique<strategies::BeetleMoveStrategy>(),
                    owner,
                    position) {
        }

        /**************************************************************************************************
         * Destructor
         *************************************************************************************************/

        /**
         * @brief Default destructor for the Beetle piece.
         *
         * Ensures proper cleanup of resources specific to the Beetle piece. Declared as `override`
         * to enforce polymorphic deletion for derived classes.
         */
        ~Beetle() override = default;

        /**************************************************************************************************
         * Other constructors and operators
         *************************************************************************************************/

        /**
         * @brief Delete copy constructor.
         */
        Beetle(const Beetle &) = delete;

        /**
         * @brief Delete copy assignment operator.
         */
        Beetle &operator=(const Beetle &) = delete;

        /**
         * @brief Default move constructor.
         */
        Beetle(Beetle &&) noexcept = default;

        /**
         * @brief Default move assignment operator.
         */
        Beetle &operator=(Beetle &&) noexcept = default;
    };
} // namespace hive::models::pieces

#endif // BEETLE_H

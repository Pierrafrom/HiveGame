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
        /**************************************************************************************************
         * Constructors
         *************************************************************************************************/

        /**
         * @brief Constructs a QueenBee piece with an optional specified ID, owner, and position.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         * @param owner Shared pointer to the owning player (default is nullptr).
         * @param position Optional initial position of the piece on the board (default is std::nullopt).
         *
         * This constructor initializes a QueenBee piece, assigning a `QueenBeeMoveStrategy` for its unique movement.
         */
        explicit QueenBee(const size_t id = pieceNextId++,
                          const std::shared_ptr<Player> &owner = nullptr,
                          const std::optional<Hex> &position = std::nullopt)
            : Piece(id,
                    enums::PieceType::QUEEN_BEE,
                    std::make_unique<strategies::QueenBeeMoveStrategy>(),
                    owner,
                    position) {
        }

        /**************************************************************************************************
         * Destructor
         *************************************************************************************************/

        /**
         * @brief Default destructor for the QueenBee piece.
         *
         * Ensures proper cleanup of resources specific to the QueenBee piece. Declared as `override`
         * to enforce polymorphic deletion for derived classes.
         */
        ~QueenBee() override = default;

        /**************************************************************************************************
         * Other constructors and operators
         *************************************************************************************************/

        /**
         * @brief Delete copy constructor.
         */
        QueenBee(const QueenBee &) = delete;

        /**
         * @brief Delete copy assignment operator.
         */
        QueenBee &operator=(const QueenBee &) = delete;

        /**
         * @brief Default move constructor.
         */
        QueenBee(QueenBee &&) noexcept = default;

        /**
         * @brief Default move assignment operator.
         */
        QueenBee &operator=(QueenBee &&) noexcept = default;
    };
} // namespace hive::models::pieces

#endif // QUEENBEE_H

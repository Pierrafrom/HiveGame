#ifndef SPIDER_H
#define SPIDER_H

#include "models/Piece.h"
#include "models/strategies/SpiderMoveStrategy.h"

namespace hive::models::pieces {
    /**
     * @class Spider
     * @brief Represents a Spider piece in the Hive game.
     *
     * The `Spider` class inherits from the abstract `Piece` class and encapsulates
     * the unique characteristics and movement behavior of the Spider piece in Hive.
     * The Spider can only move exactly three spaces along the edge of the hive.
     */
    class Spider final : public Piece {
    public:
        /**************************************************************************************************
         * Constructors
         *************************************************************************************************/

        /**
         * @brief Constructs a Spider piece with an optional specified ID, owner, and position.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         * @param owner Shared pointer to the owning player (default is nullptr).
         * @param position Optional initial position of the piece on the board (default is std::nullopt).
         *
         * This constructor initializes a Spider piece, assigning a `SpiderMoveStrategy` for its unique movement.
         */
        explicit Spider(const size_t id = pieceNextId++,
                        const std::shared_ptr<Player> &owner = nullptr,
                        const std::optional<Hex> &position = std::nullopt)
            : Piece(id,
                    enums::PieceType::SPIDER,
                    std::make_unique<strategies::SpiderMoveStrategy>(this),
                    owner,
                    position) {
        }

        /**************************************************************************************************
         * Destructor
         *************************************************************************************************/

        /**
         * @brief Default destructor for the Spider piece.
         *
         * Ensures proper cleanup of resources specific to the Spider piece. Declared as `override`
         * to enforce polymorphic deletion for derived classes.
         */
        ~Spider() override = default;

        /**************************************************************************************************
         * Other constructors and operators
         *************************************************************************************************/

        /**
         * @brief Delete copy constructor.
         */
        Spider(const Spider &) = delete;

        /**
         * @brief Delete copy assignment operator.
         */
        Spider &operator=(const Spider &) = delete;

        /**
         * @brief Default move constructor.
         */
        Spider(Spider &&) noexcept = default;

        /**
         * @brief Default move assignment operator.
         */
        Spider &operator=(Spider &&) noexcept = default;
    };
} // namespace hive::models::pieces

#endif // SPIDER_H

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
        /**
         * @brief Constructor for the Spider piece with an optional specified ID.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         *
         * This constructor initializes a Spider piece, assigning a `SpiderMoveStrategy` for its unique movement.
         */
        explicit Spider(const size_t id = nextId++) : Piece(id, enums::PieceType::SPIDER,
                                                            std::make_unique<strategies::SpiderMoveStrategy>()) {
        }

        /**
         * @brief Destructor for the Spider piece.
         *
         * The destructor is defaulted as no special cleanup is necessary.
         */
        ~Spider() override = default;
    };
} // namespace hive::models::pieces

#endif // SPIDER_H

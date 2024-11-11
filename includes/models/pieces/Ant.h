#ifndef ANT_H
#define ANT_H

#include "models/Piece.h"
#include "models/strategies/AntMoveStrategy.h"

namespace hive::models::pieces {
    /**
     * @class Ant
     * @brief Represents an Ant piece in the Hive game.
     *
     * The `Ant` class inherits from the abstract `Piece` class and encapsulates
     * the unique characteristics and movement behavior of the Ant piece in Hive.
     * Ants have unique movement rules that allow them to traverse multiple spaces
     * on the board in a single move, provided they remain adjacent to other pieces.
     */
    class Ant final : public Piece {
    public:
        /**
         * @brief Constructor for the Ant piece with an optional specified ID.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         *
         * This constructor initializes an Ant piece, assigning an `AntMoveStrategy` for the Ant's unique movement.
         * If no ID is provided, it defaults to an auto-incremented ID.
         */
        explicit Ant(const size_t id = nextId++) : Piece(id, enums::PieceType::ANT,
                                                         std::make_unique<strategies::AntMoveStrategy>()) {
        }

        /**
         * @brief Destructor for the Ant piece.
         *
         * The destructor is defaulted as no special cleanup is necessary. It is declared
         * as `override` to enforce polymorphic deletion for derived classes.
         */
        ~Ant() override = default;
    };
} // namespace hive::models::pieces

#endif // ANT_H

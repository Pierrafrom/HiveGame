#ifndef MOSQUITO_H
#define MOSQUITO_H

#include "models/Piece.h"
#include "models/strategies/MosquitoMoveStrategy.h"

namespace hive::models::pieces {
    /**
     * @class Mosquito
     * @brief Represents a Mosquito piece in the Hive game.
     *
     * The `Mosquito` class inherits from the abstract `Piece` class and copies
     * the movement abilities of adjacent pieces in the hive.
     */
    class Mosquito final : public Piece {
    public:
        /**
         * @brief Constructor for the Mosquito piece with an optional specified ID.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         *
         * This constructor initializes a Mosquito piece, assigning a `MosquitoMoveStrategy` for its movement,
         * which mimics adjacent pieces.
         */
        explicit Mosquito(const size_t id = pieceNextId++) : Piece(id, enums::PieceType::MOSQUITO,
                                                              std::make_unique<strategies::MosquitoMoveStrategy>()) {
        }

        /**
         * @brief Destructor for the Mosquito piece.
         *
         * The destructor is defaulted as no special cleanup is necessary.
         */
        ~Mosquito() override = default;
    };
} // namespace hive::models::pieces

#endif // MOSQUITO_H

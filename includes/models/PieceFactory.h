#ifndef PIECE_FACTORY_H
#define PIECE_FACTORY_H

#include <memory>
#include "models/Piece.h"
#include "models/enums/PieceType.h"

namespace hive::models {
    /**
     * @class PieceFactory
     * @brief Factory class for creating game pieces in the Hive game.
     *
     * The `PieceFactory` class provides a centralized mechanism for creating
     * various types of pieces in the Hive game. It ensures consistent and
     * flexible piece creation while encapsulating construction logic.
     */
    class PieceFactory {
    public:
        /**
         * @brief Creates a piece based on the specified type with optional attributes.
         * @param type The type of piece to create (e.g., QueenBee, Ant).
         * @param id Optional unique identifier for the piece. Defaults to 0 (indicating no specific ID).
         * @return A unique pointer to the created piece.
         *
         * This method selects the appropriate constructor for the piece based on
         * whether an ID is provided or not.
         */
        static std::unique_ptr<Piece> createPiece(enums::PieceType type, size_t id = 0);
    };
} // namespace hive::models

#endif // PIECE_FACTORY_H

#ifndef PIECE_FACTORY_H
#define PIECE_FACTORY_H

#include <memory>
#include "models/Piece.h"
#include "models/enums/PieceType.h"

//TODO: Manage maximum number of pieces

namespace hive::models {
    /**
     * @class PieceFactory
     * @brief Factory class for creating game pieces in the Hive game.
     *
     * The `PieceFactory` class provides a centralized mechanism for creating
     * various types of pieces in the Hive game. It encapsulates the logic
     * necessary to create each type of piece, ensuring consistency and
     * facilitating code maintainability.
     */
    class PieceFactory {
    public:
        /**
         * @brief Creates a piece based on the specified type with an optional ID.
         * @param type The type of piece to create (e.g., QueenBee, Ant).
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         * @return A unique pointer to the created piece.
         *
         * This function creates and returns a unique pointer to the requested piece
         * type. If an ID is provided, it assigns that ID to the piece; otherwise,
         * an auto-incremented ID is generated.
         */
        static std::unique_ptr<Piece> createPiece(enums::PieceType type, size_t id = 0);
    };
} // namespace hive::models

#endif // PIECE_FACTORY_H

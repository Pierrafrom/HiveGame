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
        /**************************************************************************************************
         * Constructors
         *************************************************************************************************/

        /**
         * @brief Constructs a Mosquito piece with an optional specified ID, owner, and position.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         * @param owner Shared pointer to the owning player (default is nullptr).
         * @param position Optional initial position of the piece on the board (default is std::nullopt).
         *
         * This constructor initializes a Mosquito piece, assigning a `MosquitoMoveStrategy` for its movement,
         * which mimics adjacent pieces.
         */
        explicit Mosquito(const size_t id = pieceNextId++,
                          const std::shared_ptr<Player> &owner = nullptr,
                          const std::optional<Hex> &position = std::nullopt)
            : Piece(id,
                    enums::PieceType::MOSQUITO,
                    std::make_unique<strategies::MosquitoMoveStrategy>(),
                    owner,
                    position) {
        }

        /**************************************************************************************************
         * Destructor
         *************************************************************************************************/

        /**
         * @brief Default destructor for the Mosquito piece.
         *
         * Ensures proper cleanup of resources specific to the Mosquito piece. Declared as `override`
         * to enforce polymorphic deletion for derived classes.
         */
        ~Mosquito() override = default;

        /**************************************************************************************************
         * Other constructors and operators
         *************************************************************************************************/

        /**
         * @brief Delete copy constructor.
         */
        Mosquito(const Mosquito &) = delete;

        /**
         * @brief Delete copy assignment operator.
         */
        Mosquito &operator=(const Mosquito &) = delete;

        /**
         * @brief Default move constructor.
         */
        Mosquito(Mosquito &&) noexcept = default;

        /**
         * @brief Default move assignment operator.
         */
        Mosquito &operator=(Mosquito &&) noexcept = default;
    };
} // namespace hive::models::pieces

#endif // MOSQUITO_H

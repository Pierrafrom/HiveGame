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
        /**************************************************************************************************
         * Constructors
         *************************************************************************************************/

        /**
         * @brief Constructs an Ant piece with an optional specified ID, owner, and position.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         * @param owner Shared pointer to the owning player (default is nullptr).
         * @param position Optional initial position of the piece on the board (default is std::nullopt).
         *
         * This constructor initializes an Ant piece, assigning an `AntMoveStrategy` for the Ant's unique movement.
         */
        explicit Ant(const size_t id = pieceNextId++,
                     const std::shared_ptr<Player> &owner = nullptr,
                     const std::optional<Hex> &position = std::nullopt)
            : Piece(id,
                    enums::PieceType::ANT,
                    std::make_unique<strategies::AntMoveStrategy>(this),
                    owner,
                    position) {
        }

        /**************************************************************************************************
         * Destructor
         *************************************************************************************************/

        /**
         * @brief Default destructor for the Ant piece.
         *
         * Ensures proper cleanup of resources specific to the Ant piece. Declared as `override`
         * to enforce polymorphic deletion for derived classes.
         */
        ~Ant() override = default;

        /**************************************************************************************************
         * Other constructors and operators
         *************************************************************************************************/

        /**
         * @brief Delete copy constructor.
         */
        Ant(const Ant &) = delete;

        /**
         * @brief Delete copy assignment operator.
         */
        Ant &operator=(const Ant &) = delete;

        /**
         * @brief Default move constructor.
         */
        Ant(Ant &&) noexcept = default;

        /**
         * @brief Default move assignment operator.
         */
        Ant &operator=(Ant &&) noexcept = default;
    };
} // namespace hive::models::pieces

#endif // ANT_H

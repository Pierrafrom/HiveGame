#ifndef PIECE_H
#define PIECE_H

#include <cstddef>
#include <memory>
#include "models/enums/PieceType.h"
#include "models/strategies/MoveStrategy.h"

namespace hive::models {
    /**
     * @class Piece
     * @brief Base class representing a generic game piece in the Hive game.
     *
     * This class serves as the foundation for all specific types of game pieces in Hive.
     * It contains attributes and methods that are common to all pieces, such as unique
     * identifiers, type information, and movement strategies.
     */
    class Piece {
    protected:
        /**
         * @brief Static counter used for auto-incrementing unique piece IDs.
         *
         * Each new piece created without an explicit ID will be assigned a unique ID
         * based on this counter. It ensures that all pieces have distinct identifiers.
         */
        static size_t nextId;

        /**
         * @brief Unique identifier for the piece.
         *
         * This ID is unique across all instances of Piece and can either be auto-generated
         * or explicitly provided.
         */
        size_t id;

        /**
         * @brief Type of the piece (e.g., QueenBee, Ant).
         *
         * Represents the specific type of insect for each piece, using an enum defined
         * in `PieceType`.
         */
        enums::PieceType type;

        /**
         * @brief Movement strategy for the piece.
         *
         * Each piece may have a unique movement strategy depending on its type. This
         * attribute holds a pointer to a strategy object that defines how the piece moves.
         */
        std::unique_ptr<strategies::MoveStrategy> moveStrategy;

        /**
         * @brief Constructor for a piece with either an auto-incremented ID or a specified ID.
         * @param id Optional unique identifier for the piece. If not provided, an auto-incremented ID is assigned.
         * @param type The specific type of the piece (e.g., QueenBee, Ant).
         * @param strategy A unique pointer to the MoveStrategy object (default is nullptr).
         *
         * This constructor can assign a unique ID to the piece using the `nextId` counter, or it can accept
         * a specific ID, allowing for flexibility in ID assignment.
         */
        explicit Piece(const size_t id, const enums::PieceType type,
                       std::unique_ptr<strategies::MoveStrategy> strategy = nullptr)
            : id(id), type(type), moveStrategy(std::move(strategy)) {
        }

    public:
        /**
         * @brief Virtual destructor.
         *
         * Ensures that derived classes can override and properly clean up resources if needed.
         */
        virtual ~Piece() = default;

        /**
         * @brief Retrieves the unique identifier of the piece.
         * @return The unique ID of the piece.
         */
        [[nodiscard]] size_t getId() const { return id; }

        /**
         * @brief Retrieves the type of the piece.
         * @return The type of the piece as defined in the PieceType enum.
         */
        [[nodiscard]] enums::PieceType getType() const { return type; }

        /**
         * @brief Sets the movement strategy for the piece.
         * @param strategy A unique pointer to the MoveStrategy object.
         *
         * This method allows assigning a specific movement strategy to the piece, enabling
         * polymorphic movement behavior depending on the type of piece.
         */
        void setMoveStrategy(std::unique_ptr<strategies::MoveStrategy> strategy) { moveStrategy = std::move(strategy); }

        /**
         * @brief Retrieves the movement strategy of the piece.
         * @return A reference to the MoveStrategy object.
         *
         * Provides access to the piece's movement strategy, allowing external classes to
         * determine how the piece can move on the board.
         */
        [[nodiscard]] const strategies::MoveStrategy &getMoveStrategy() const { return *moveStrategy; }

        /**
         * @brief Equality operator for comparing two pieces.
         * @param other The other piece to compare with.
         * @return True if both pieces have the same ID, false otherwise.
         */
        bool operator==(const Piece &other) const { return id == other.id; }

        /**
         * @brief Inequality operator for comparing two pieces.
         * @param other The other piece to compare with.
         * @return True if the pieces differ in ID, false otherwise.
         */
        bool operator!=(const Piece &other) const { return !(*this == other); }
    };
} // namespace hive::models

#endif // PIECE_H

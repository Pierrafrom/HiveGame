#include "models/Piece.h"

#include <models/Board.h>
#include <models/Player.h>

namespace hive::models {
    size_t Piece::pieceNextId = 1; // Initialize static ID counter

    /*******************************************************************************************************************
     * Constructors
     ******************************************************************************************************************/

    // Constructor implementation
    Piece::Piece(const size_t id,
                 const enums::PieceType type,
                 std::unique_ptr<strategies::MoveStrategy> strategy,
                 const std::shared_ptr<Player> &owner,
                 const std::optional<Hex> &position)
        : id(id),
          type(type),
          moveStrategy(std::move(strategy)),
          owner(owner),
          position(position) {
        if (id >= pieceNextId) {
            pieceNextId = id + 1; // Update the static ID counter if necessary
        }

        if (owner) {
            owner->addPiece(std::shared_ptr<Piece>(this));
        }
    }

    // Move constructor implementation
    Piece::Piece(Piece &&other) noexcept
        : id(other.id),
          type(other.type),
          moveStrategy(std::move(other.moveStrategy)),
          owner(std::move(other.owner)),
          position(std::move(other.position)) {
    }

    // Move assignment operator implementation
    Piece &Piece::operator=(Piece &&other) noexcept {
        if (this != &other) {
            id = other.id;
            type = other.type;
            moveStrategy = std::move(other.moveStrategy);
            owner = std::move(other.owner);
            position = std::move(other.position);
        }
        return *this;
    }

    /*******************************************************************************************************************
     * Getters
     ******************************************************************************************************************/

    // Retrieves the movement strategy of the piece
    const strategies::MoveStrategy &Piece::getMoveStrategy() const {
        if (!moveStrategy) {
            throw std::runtime_error("Move strategy not set for this piece.");
        }
        return *moveStrategy;
    }

    // Retrieves the player that owns the piece
    const Player &Piece::getOwner() const {
        if (const auto ownerPtr = owner.lock()) {
            return *ownerPtr;
        }
        throw std::runtime_error("This piece does not have an owner.");
    }

    /*******************************************************************************************************************
     * Setters
     ******************************************************************************************************************/

    // Retrieves the current position of the piece
    void Piece::setMoveStrategy(std::unique_ptr<strategies::MoveStrategy> strategy) {
        if (!strategy) {
            throw std::invalid_argument("Cannot set a null move strategy.");
        }
        moveStrategy = std::move(strategy);
    }

    // Sets the player that owns the piece
    void Piece::setOwner(const std::weak_ptr<Player> &newOwner) {
        if (auto sharedOwner = newOwner.lock()) {
            owner = newOwner;
        } else {
            throw std::invalid_argument("Owner cannot be null or expired.");
        }
    }


    // Sets the current position of the piece
    void Piece::setPosition(const Hex &newPosition) {
        position = newPosition;
    }

    /*******************************************************************************************************************
     * Operators
     ******************************************************************************************************************/

    bool Piece::operator==(const Piece &other) const {
        return id == other.id && type == other.type && owner.lock() == other.owner.lock();
    }

    std::ostream &operator<<(std::ostream &os, const Piece &piece) {
        os << piece.toString();
        return os;
    }

    /*******************************************************************************************************************
     * Public methods
     ******************************************************************************************************************/

    // Retrieves the neighboring pieces of the current piece
    std::vector<std::shared_ptr<const Piece> > Piece::getNeighbors(const Board &board) const {
        std::vector<std::shared_ptr<const Piece> > neighbors;

        if (!position) {
            return neighbors; // No position, no neighbors
        }

        for (const auto &neighborHex: board.neighbors(position.value())) {
            if (auto neighborPiece = board.getTopPiece(neighborHex)) {
                neighbors.push_back(std::const_pointer_cast<const Piece>(neighborPiece));
            }
        }

        return neighbors;
    }

    std::string Piece::toString() const {
        const std::string ownerStr = owner.expired() ? "None" : std::to_string(owner.lock()->getId());
        return "Piece(ID: " + std::to_string(id) +
               ", Type: " + std::to_string(static_cast<int>(type)) +
               ", Owner: " + ownerStr +
               ", Position: " + (position ? position->toString() : "None") + ")";
    }
} // namespace hive::models

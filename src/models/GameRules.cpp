#include "models/GameRules.h"

#include "models/Board.h"
#include "config/constants.h"
#include <stdexcept>

namespace hive::models {
    /**************************************************************************************************
     * Singleton Implementation
     *************************************************************************************************/

    // Get the singleton instance
    GameRules &GameRules::getInstance() {
        static GameRules instance;
        return instance;
    }

    /**************************************************************************************************
     * Public Methods
     *************************************************************************************************/

    // Check if the move is valid
    bool GameRules::validateMove(const Move &move, const Board &board) {
        if (move.getType() == Move::MoveType::PLACE) {
            return validatePlaceType(move, board);
        }
        if (move.getType() == Move::MoveType::MOVE) {
            return validateMoveType(move, board);
        }
        throw std::runtime_error("Invalid move type.");
    }

    // Check if the player can add a piece of the given type
    bool GameRules::canAddPiece(const Player &player, const enums::PieceType type) {
        const int pieceCount = player.getPieceCount(type);

        switch (type) {
            case enums::PieceType::QUEEN_BEE:
                return pieceCount < config::constants::MAX_PIECES.at(enums::PieceType::QUEEN_BEE);
            case enums::PieceType::ANT:
                return pieceCount < config::constants::MAX_PIECES.at(enums::PieceType::ANT);
            case enums::PieceType::BEETLE:
                return pieceCount < config::constants::MAX_PIECES.at(enums::PieceType::BEETLE);
            case enums::PieceType::LADYBUG:
                return pieceCount < config::constants::MAX_PIECES.at(enums::PieceType::LADYBUG);
            case enums::PieceType::MOSQUITO:
                return pieceCount < config::constants::MAX_PIECES.at(enums::PieceType::MOSQUITO);
            case enums::PieceType::SPIDER:
                return pieceCount < config::constants::MAX_PIECES.at(enums::PieceType::SPIDER);
            case enums::PieceType::PILLBUG:
                return pieceCount < config::constants::MAX_PIECES.at(enums::PieceType::PILLBUG);
            case enums::PieceType::GRASSHOPPER:
                return pieceCount < config::constants::MAX_PIECES.at(enums::PieceType::GRASSHOPPER);
            default:
                throw std::runtime_error("Unknown piece type.");
        }
    }

    // Check if the player has to place the queen bee
    bool GameRules::isQueenPlacementRequired(const Player &player, const size_t turnNumber) {
        if (turnNumber == config::constants::MAX_TURN_BEFORE_QUEEN_PLACEMENT) {
            return player.getPieceCount(enums::PieceType::QUEEN_BEE) == 0;
        }
        return false;
    }

    /**************************************************************************************************
     * Private Methods
     *************************************************************************************************/

    // validate that the piece can be moved (e.g., piece is owned by the player)
    bool GameRules::validateMoveType(const Move &move, const Board &board) {
        //! WARNING: All logic regarding the possibility of moving a piece will be handled by the getPossibleMoves
        // method of the MoveStrategy class. Therefore, it is important to ensure that this method is called before or
        // after validateMoves.

        // check if the starting hex is occupied
        if (!board.isOccupied(move.getFrom().value())) {
            throw std::invalid_argument("Starting hex is not occupied.");
        }

        // check if the piece is owned by the player
        if (const auto &piece = board.getTopPiece(move.getFrom().value());
            piece->getOwner() != *move.getPlayer()) {
            throw std::invalid_argument("Piece is not owned by the player.");
        }

        // check if the to hex is occupied and if the piece is not a beetle
        if (board.isOccupied(move.getTo())) {
            if (const auto &piece = board.getTopPiece(move.getTo());
                piece->getType() != enums::PieceType::BEETLE) {
                throw std::invalid_argument("Target hex is already occupied.");
            }
        }

        return true;
    }

    // validate that the piece can be placed (e.g., adjacency rules)
    bool GameRules::validatePlaceType(const Move &move, const Board &board) {
        // Validate that the target hex is not occupied
        if (board.isOccupied(move.getTo())) {
            throw std::invalid_argument("Target hex is already occupied.");
        }

        // Validate that the piece can be placed (e.g., adjacency rules)
        bool hasAdjacentPieces = false;
        for (const auto &neighbor: board.getNeighborHexes(move.getTo())) {
            if (board.isOccupied(neighbor)) {
                hasAdjacentPieces = true;
                break;
            }
        }

        if (!hasAdjacentPieces) {
            throw std::invalid_argument("Placement hex must be adjacent to existing pieces.");
        }

        // validate that the placement hex is not adjacent to an opponent's piece
        for (const auto &neighbor: board.getNeighborHexes(move.getTo())) {
            if (board.isOccupied(neighbor)) {
                if (const auto &piece = board.getTopPiece(neighbor);
                    piece->getOwner() != *move.getPlayer()) {
                    throw std::invalid_argument("Placement hex must not be adjacent to an opponent's piece.");
                }
            }
        }

        return true;
    }

    // Check victory conditions and return the winner or draw state
    std::optional<const Player *>
    GameRules::getVictoryCondition(const Board &board, const std::vector<Player> &players) {
        std::vector<const Player *> surroundedPlayers; // Track players with surrounded queen bees

        for (const auto &player: players) {
            // Retrieve the queen bee pieces owned by the player
            const auto queenBeePieces = player.getPieces(enums::PieceType::QUEEN_BEE);

            // Skip this player if they do not have a queen bee piece (e.g., early game)
            if (queenBeePieces.empty()) {
                continue;
            }

            // Assume there's only one queen bee per player and get its position
            const auto &queenBee = queenBeePieces[0];
            const auto queenBeePosOpt = queenBee->getPosition();

            // If the queen bee has no position, skip (e.g., not yet placed on the board)
            if (!queenBeePosOpt.has_value()) {
                continue;
            }

            const Hex &queenBeePos = queenBeePosOpt.value();

            // Check if all neighboring hexes are occupied
            bool isSurrounded = true;
            for (const auto &neighbor: board.getNeighborHexes(queenBeePos)) {
                if (!board.isOccupied(neighbor)) {
                    isSurrounded = false;
                    break;
                }
            }

            // If the queen bee is surrounded, add the player to the list
            if (isSurrounded) {
                surroundedPlayers.push_back(&player);
            }
        }

        // Determine the game state based on the number of surrounded players
        if (surroundedPlayers.size() == 1) {
            // If only one player's queen bee is surrounded, they lose, so the other wins
            for (const auto &player: players) {
                if (std::find(surroundedPlayers.begin(), surroundedPlayers.end(), &player) == surroundedPlayers.end()) {
                    return &player; // Return the winning player
                }
            }
        } else if (surroundedPlayers.size() == players.size()) {
            // If all players' queen bees are surrounded, it's a draw
            return std::nullopt; // Indicate a draw
        }

        // If no queen bees are surrounded or not enough information, no winner yet
        return nullptr;
    }
} // namespace hive::models

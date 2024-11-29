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
        // method of the MoveStrategy class. Therefore, it is important to ensure that this method is called before
        // validateMoves.

        // check if the starting hex is occupied
        if (!board.isOccupied(move.getFrom())) {
            throw std::invalid_argument("Starting hex is not occupied.");
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
                if (!move.getPlayer()->ownsPiece(*board.getTopPiece(neighbor))) {
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
            const auto &queenBeePieces = player.getPieces(enums::PieceType::QUEEN_BEE);

            // Skip this player if they have no queen bee (e.g., early game)
            if (queenBeePieces.empty()) {
                continue;
            }

            // Get the position of the player's queen bee
            const Hex queenBeePos = board.getPiecePosition(queenBeePieces[0]);

            // Check if the queen bee is surrounded
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

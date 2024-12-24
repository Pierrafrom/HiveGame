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
    void GameRules::validateMove(const Move &move, const Board &board, const size_t turnNumber) {
        try {
            if (move.getType() == Move::MoveType::PLACE || move.getType() == Move::MoveType::MOVE) {
                // We don't need to check if the player is null because it is checked in the Move constructor
                // We don't need to check if the piece is null because it is checked in the Move constructor
                if (isQueenPlacementRequired(*move.getPlayer(), turnNumber)) {
                    if (move.getPiece()->getType() != enums::PieceType::QUEEN_BEE &&
                        move.getType() == Move::MoveType::PLACE) {
                        throw std::invalid_argument("Queen bee placement is required.");
                    }
                }
            }

            if (move.getType() == Move::MoveType::PLACE) {
                validatePlaceType(move, board);
                return;
            }

            if (move.getType() == Move::MoveType::MOVE) {
                validateMoveType(move, board);
                return;
            }

            throw std::runtime_error("Invalid move type.");
        } catch (const std::exception &e) {
            throw std::runtime_error("Move validation failed: " + std::string(e.what()));
        }
    }

    // Check victory conditions and return the winner or draw state
    std::optional<const Player *> GameRules::getVictoryCondition(
        const Board &board, const std::vector<const Player *> &players) {
        std::vector<const Player *> surroundedPlayers; // Track players whose queen bees are surrounded

        // Iterate over all players to check their queen bee status
        for (const auto *player: players) {
            if (!player) {
                throw std::invalid_argument("Player pointer is null.");
            }

            // Retrieve the queen bee pieces owned by the player
            const auto queenBeePieces = player->getPieces(enums::PieceType::QUEEN_BEE);

            // Skip this player if they do not have a queen bee piece (e.g., early game)
            if (queenBeePieces.empty()) {
                continue;
            }

            // Assume there's only one queen bee per player and get its position
            const auto &queenBee = queenBeePieces[0];
            const auto queenBeePosOpt = queenBee->getPosition();

            // If the queen bee has no position (not yet placed), skip this player
            if (!queenBeePosOpt.has_value()) {
                continue;
            }

            const Hex &queenBeePos = queenBeePosOpt.value();

            // Check if all neighboring hexes around the queen bee are occupied
            bool isSurrounded = true;
            for (const auto &neighbor: board.neighbors(queenBeePos)) {
                if (!board.isOccupied(neighbor)) {
                    isSurrounded = false;
                    break; // Exit early if a free neighbor is found
                }
            }

            // If the queen bee is surrounded, add this player to the list
            if (isSurrounded) {
                surroundedPlayers.push_back(player);
            }
        }

        // Determine the game state based on the number of surrounded players
        if (surroundedPlayers.size() == 1) {
            // If only one player's queen bee is surrounded, they lose, so the other player wins
            for (const auto *player: players) {
                if (std::find(surroundedPlayers.begin(), surroundedPlayers.end(), player) == surroundedPlayers.end()) {
                    return player; // Return the pointer to the winning player
                }
            }
        } else if (surroundedPlayers.size() == players.size()) {
            // If all players' queen bees are surrounded, it's a draw
            return std::nullopt; // Indicate a draw state
        }

        // If no queen bees are surrounded or the game is not over, return nullptr
        return nullptr;
    }


    /**************************************************************************************************
     * Private Methods
     *************************************************************************************************/

    // validate that the piece can be moved (e.g., piece is owned by the player)
    void GameRules::validateMoveType(const Move &move, const Board &board) {
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
            if (const auto &piece = board.getTopPiece(move.getFrom().value());
                piece->getType() != enums::PieceType::BEETLE) {
                throw std::invalid_argument("Target hex is already occupied.");
            }
        }
    }

    // validate that the piece can be placed (e.g., adjacency rules)
    void GameRules::validatePlaceType(const Move &move, const Board &board) {
        if (!canAddPiece(*move.getPlayer(), move.getPiece()->getType())) {
            throw std::invalid_argument("Player cannot add more pieces of this type.");
        }

        if (board.isOccupied(move.getTo()) && move.getPiece()->getType() != enums::PieceType::BEETLE) {
            throw std::invalid_argument("Target hex is already occupied.");
        }

        if (board.pieceCount() > 1) {
            // this rules is only valid after the first piece of each player is placed
            // validate that the placement hex is not adjacent to an opponent's piece
            for (const auto &neighbor: board.neighbors(move.getTo())) {
                if (board.isOccupied(neighbor)) {
                    if (const auto &piece = board.getTopPiece(neighbor);
                        piece->getOwner() != *move.getPlayer()) {
                        throw std::invalid_argument("Placement hex must not be adjacent to an opponent's piece.");
                    }
                }
            }
        }
    }

    // Check if the player can add a piece of the given type
    bool GameRules::canAddPiece(const Player &player, const enums::PieceType type) {
        const int pieceCount = player.getPieceCount(type);

        switch (type) {
            case enums::PieceType::QUEEN_BEE:
                return pieceCount <= config::constants::MAX_PIECES.at(enums::PieceType::QUEEN_BEE);
            case enums::PieceType::ANT:
                return pieceCount <= config::constants::MAX_PIECES.at(enums::PieceType::ANT);
            case enums::PieceType::BEETLE:
                return pieceCount <= config::constants::MAX_PIECES.at(enums::PieceType::BEETLE);
            case enums::PieceType::LADYBUG:
                return pieceCount <= config::constants::MAX_PIECES.at(enums::PieceType::LADYBUG);
            case enums::PieceType::MOSQUITO:
                return pieceCount <= config::constants::MAX_PIECES.at(enums::PieceType::MOSQUITO);
            case enums::PieceType::SPIDER:
                return pieceCount <= config::constants::MAX_PIECES.at(enums::PieceType::SPIDER);
            case enums::PieceType::PILLBUG:
                return pieceCount <= config::constants::MAX_PIECES.at(enums::PieceType::PILLBUG);
            case enums::PieceType::GRASSHOPPER:
                return pieceCount <= config::constants::MAX_PIECES.at(enums::PieceType::GRASSHOPPER);
            default:
                throw std::runtime_error("Unknown piece type.");
        }
    }

    // Check if the player has to place the queen bee
    bool GameRules::isQueenPlacementRequired(const Player &player, const size_t turnNumber) {
        if (turnNumber >= config::constants::MAX_TURN_BEFORE_QUEEN_PLACEMENT) {
            return player.getPieceCount(enums::PieceType::QUEEN_BEE) == 0;
        }
        return false;
    }
} // namespace hive::models

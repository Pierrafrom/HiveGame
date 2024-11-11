#ifndef GAMERULES_H
#define GAMERULES_H

#include "models/Board.h"
#include "models/Player.h"
#include "models/Move.h"
#include <vector>

//TODO: implement the GameRules class
//TODO: create a test suite for the GameRules class

namespace hive::models {
    /**
     * @class GameRules
     * @brief Encapsulates the rules of the Hive game.
     *
     * The GameRules class provides methods to validate moves, enforce the game rules
     * (such as queen placement before the fourth turn, maximum number of pieces per player),
     * and check for game-ending conditions.
     */
    class GameRules {
    public:
        /**
         * @brief Validates a move according to the game rules.
         * @param move The move to validate.
         * @param board The current game board.
         * @param player The player making the move.
         * @param turnNumber The current turn number.
         * @return True if the move is valid, false otherwise.
         */
        [[nodiscard]] bool validateMove(const Move &move, const Board &board, const Player &player,
                                        size_t turnNumber) const;

        /**
         * @brief Checks if a piece placement is valid according to the adjacency rules.
         * @param move The move containing the position where the piece will be placed.
         * @param board The current game board.
         * @param player The player attempting to place the piece.
         * @return True if the placement is valid, false otherwise.
         */
        [[nodiscard]] bool validatePlacement(const Move &move, const Board &board, const Player &player) const;

        /**
         * @brief Checks if the player can add a specific piece type according to the max limit.
         * @param player The player attempting to add the piece.
         * @param type The type of the piece to add.
         * @return True if the player can add the piece, false otherwise.
         */
        [[nodiscard]] bool canAddPiece(const Player &player, enums::PieceType type) const;

        /**
         * @brief Enforces the rule that the queen must be placed by the fourth turn.
         * @param player The player to check.
         * @param turnNumber The current turn number.
         * @return True if the queen placement is required, false otherwise.
         */
        [[nodiscard]] bool isQueenPlacementRequired(const Player &player, size_t turnNumber) const;

        /**
         * @brief Ensures the Hive remains connected after a move.
         * @param move The move to validate for Hive continuity.
         * @param board The current game board.
         * @return True if the Hive remains connected, false otherwise.
         */
        [[nodiscard]] bool isHiveConnectedAfterMove(const Move &move, const Board &board) const;

        /**
         * @brief Checks if the win condition is met (one queen is fully surrounded).
         * @param board The current game board.
         * @param players The players in the game.
         * @return True if the game has a winner, false otherwise.
         */
        [[nodiscard]] bool isVictoryConditionMet(const Board &board, const std::vector<Player> &players) const;

        /**
         * @brief Checks if a stalemate condition is met (both queens are fully surrounded).
         * @param players The players in the game.
         * @return True if the game is a draw, false otherwise.
         */
        [[nodiscard]] bool isStalemate(const std::vector<Player> &players) const;

        /**
         * @brief Determines if the game is over by checking victory and stalemate conditions.
         * @param board The current game board.
         * @param players The players in the game.
         * @return True if the game is over, false otherwise.
         */
        [[nodiscard]] bool isGameOver(const Board &board, const std::vector<Player> &players) const;

        /**
         * @brief Destructor.
         */
        ~GameRules() = default;
    };
} // namespace hive::models

#endif // GAMERULES_H

#ifndef GAMERULES_H
#define GAMERULES_H

#include <optional>
#include <vector>

#include "models/Player.h"
#include "models/Move.h"

// TODO: Create a test suite for the GameRules class

namespace hive::models {
    /**
     * @class GameRules
     * @brief Singleton class encapsulating the rules of the Hive game.
     *
     * The GameRules class provides methods to validate moves, enforce game rules,
     * and check for game-ending conditions. As a singleton, it ensures that only
     * one instance of the rules exists globally.
     */
    class GameRules {
        /****************************************************************************
         * Singleton class: private constructor and destructor.
         **************************************************************************/

        /**
         * @brief Private constructor to enforce singleton pattern.
         */
        GameRules() = default;

        /**
         * @brief Destructor.
         *
         * The destructor is kept private and defaulted as no special cleanup is required.
         */
        ~GameRules() = default;

        /**
         * @brief Validates a MOVE-type move.
         * @param move The move to validate.
         * @param board The game board.
         */
        static void validateMoveType(const Move &move, const Board &board);

        /**
         * @brief Validates a PLACE-type move.
         * @param move The move to validate.
         * @param board The game board.
         */
        static void validatePlaceType(const Move &move, const Board &board);

        /**
         * @brief Checks if the player can add a specific piece type according to the maximum limit.
         * @param player The player attempting to add the piece.
         * @param type The type of the piece to add.
         * @return True if the player can add the piece, false otherwise.
         */
        [[nodiscard]] static bool canAddPiece(const Player &player, enums::PieceType type);

        /**
         * @brief Enforces the rule that the queen must be placed by the fourth turn.
         * @param player The player to check.
         * @param turnNumber The current turn number.
         * @return True if the Queen placement is required, false otherwise.
         */
        [[nodiscard]] static bool isQueenPlacementRequired(const Player &player, size_t turnNumber);

    public:
        /**
         * @brief Retrieves the singleton instance of GameRules.
         * @return A reference to the single instance of GameRules.
         *
         * This method ensures that only one instance of GameRules exists throughout
         * the application. It creates the instance on first access and returns it
         * on subsequent calls.
         */
        static GameRules &getInstance();

        /**
         * @brief Validates a move according to its type.
         * @param move The move to validate.
         * @param board The game board.
         * @param turnNumber The current turn number.
         */
        static void validateMove(const Move &move, const Board &board, size_t turnNumber);

        /**
         * @brief Checks the victory conditions in the game.
         *
         * This method evaluates the current state of the game to determine if there is a winner,
         * if the game is a draw, or if it should continue.
         *
         * - A victory is declared if only one player's queen bee is surrounded.
         * - A draw is declared if all queen bees present on the board are surrounded.
         * - If no queen bees are surrounded, the game continues without a winner.
         *
         * @param board The current game board.
         * @param players A vector of players participating in the game.
         * @return A pointer to the winning player if there is a winner,
         *         `std::nullopt` for a draw, or `nullptr` if the game continues without a winner.
         */
        static std::optional<const Player *>
        getVictoryCondition(const Board &board, const std::vector<const Player *> &players);

        /****************************************************************************
         * Singleton class: prevent copying and moving the singleton instance.
         **************************************************************************/

        /**
         * @brief Deleted copy constructor.
         *
         * This prevents copying the singleton instance.
         */
        GameRules(const GameRules &) = delete;

        /**
         * @brief Deleted assignment operator.
         *
         * This prevents assigning the singleton instance.
         */
        GameRules &operator=(const GameRules &) = delete;

        /**
         * @brief Deleted move constructor.
         *
         * This prevents moving the singleton instance.
         */
        GameRules(GameRules &&) = delete;

        /**
         * @brief Deleted move assignment operator.
         *
         * This prevents moving the singleton instance.
         */
        GameRules &operator=(GameRules &&) = delete;
    };
} // namespace hive::models

#endif // GAMERULES_H

#ifndef GAME_H
#define GAME_H

#include <array>
#include <stack>
#include <memory>
#include "models/Board.h"
#include "models/Player.h"
#include "models/Move.h"
#include "models/GameRules.h"

//TODO: Create a test suite for the Game class
//TODO: Implement the Game class
//TODO: think about the possibility of singleton for this class

namespace hive::models {
    /**
     * @class Game
     * @brief Manages the state of a Hive game, including players, board, and game logic.
     *
     * The Game class is responsible for the main game loop, turn management, executing moves,
     * undo/redo functionality, and applying game rules.
     */
    class Game {
        Board board; /**< The game board */
        std::array<Player, 2> players; /**< Array holding the two players */
        size_t currentPlayerIndex; /**< Index of the current player (0 or 1) */
        GameRules rules; /**< Game rules manager */
        std::stack<std::unique_ptr<Move> > undoStack; /**< Stack for undo operations */
        std::stack<std::unique_ptr<Move> > redoStack; /**< Stack for redo operations */
        size_t turnNumber; /**< Current turn number */

    public:
        /**
         * @brief Constructs a new game with two players.
         * @param player1 The first player.
         * @param player2 The second player.
         *
         * Initializes the game state with two players, setting the turn number to 1
         * and setting the first player as the current player.
         */
        Game(const Player &player1, const Player &player2)
            : players{player1, player2}, currentPlayerIndex(0), turnNumber(1) {
        }

        /**
         * @brief Executes a move for the current player.
         * @param move The move to be executed.
         * @return True if the move was successfully executed, otherwise false.
         *
         * Executes the specified move according to the game rules and updates
         * the board state. If successful, the move is added to the undo stack.
         */
        bool executeMove(const Move &move);

        /**
         * @brief Undoes the last move.
         * @return True if the undo operation was successful, otherwise false.
         *
         * Restores the previous game state by removing the last move.
         * The move is added to the redo stack for potential re-execution.
         */
        bool undo();

        /**
         * @brief Redoes the last undone move.
         * @return True if the redo operation was successful, otherwise false.
         *
         * Re-executes the last move that was undone, updating the game state
         * and transferring the move back to the undo stack.
         */
        bool redo();

        /**
         * @brief Checks if the game is over.
         * @return True if the game has ended, otherwise false.
         *
         * Uses the game rules to determine if a win or draw condition has been met.
         */
        bool isGameOver() const;

        /**
         * @brief Gets the current player.
         * @return Reference to the current player.
         *
         * Returns a reference to the player whose turn it is.
         */
        Player &getCurrentPlayer() { return players[currentPlayerIndex]; }

        /**
         * @brief Advances to the next turn.
         *
         * Switches to the other player and increments the turn number.
         */
        void nextTurn() {
            currentPlayerIndex = (currentPlayerIndex + 1) % 2;
            ++turnNumber;
        }

        /**
         * @brief Saves the current game state to a file.
         * @param filename The name of the file where the game state will be saved.
         * @return True if the game state was saved successfully, otherwise false.
         *
         * Serializes the game state, including board, players, and turn information,
         * into the specified file.
         */
        bool saveGame(const std::string &filename) const;

        /**
         * @brief Loads a game state from a file.
         * @param filename The name of the file to load from.
         * @return True if the game state was loaded successfully, otherwise false.
         *
         * Restores the game state from the specified file, allowing a previously saved
         * game to be resumed.
         */
        bool loadGame(const std::string &filename);

        /**
         * @brief Gets the current turn number.
         * @return The current turn number.
         */
        size_t getTurnNumber() const { return turnNumber; }

        /**
         * @brief Destructor.
         *
         * Default destructor, as no specific cleanup is required.
         */
        ~Game() = default;
    };
} // namespace hive::models

#endif // GAME_H

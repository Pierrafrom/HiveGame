#ifndef GAME_H
#define GAME_H

#include <array>
#include <stack>
#include <memory>

#include "GameRules.h"
#include "models/Board.h"
#include "models/Player.h"
#include "models/Move.h"

//TODO: Create a test suite for the Game class
//TODO: Implement the Game class
//TODO: think about the possibility of singleton for this class

namespace hive::models {
    /**
     * @struct GameStatus
     * @brief Represents the status of the game.
     */
    struct GameStatus {
        bool isGameOver; /**< Indicates if the game has ended. */
        const Player *winner; /**< Pointer to the winning player, or nullptr if the game is a draw or ongoing. */
    };

    /**
      * @class Game
      * @brief Singleton class managing the state of a Hive game.
      *
      * The Game class is responsible for the main game loop, turn management, executing moves,
      * undo/redo functionality, and applying game rules. As a singleton, it ensures only one
      * instance exists globally.
      */
    class Game {
        std::shared_ptr<Board> board; /**< The game board, managed via a shared pointer. */
        std::array<std::shared_ptr<Player>, 2> players; /**< Array holding the two players. */
        size_t currentPlayerIndex; /**< Index of the current player (0 or 1). */
        std::stack<std::unique_ptr<Move> > undoStack; /**< Stack for undo operations. */
        std::stack<std::unique_ptr<Move> > redoStack; /**< Stack for redo operations. */
        size_t turnNumber; /**< Current turn number. */

        /**
         * @brief Private constructor to enforce singleton pattern.
         * Initializes default player names.
         */
        Game()
            : board(std::make_shared<Board>()),
              players{std::make_shared<Player>("Player 1"), std::make_shared<Player>("Player 2")},
              currentPlayerIndex(0),
              turnNumber(1) {
        }

        /**
         * @brief Private destructor.
         */
        ~Game() = default;

    public:
        /**************************************************************************************************************
        * Singleton Access
        *************************************************************************************************************/

        /**
         * @brief Retrieves the single instance of the Game class.
         * @return A reference to the singleton instance of Game.
         */
        static Game &getInstance() {
            static Game instance;
            return instance;
        }

        /**
        * @brief Resets the game with new player names.
        * @param player1Name The name of the first player.
        * @param player2Name The name of the second player.
        */
        void resetGame(const std::string &player1Name = "Player 1", const std::string &player2Name = "Player 2");

        /**************************************************************************************************************
        * Deleted Copy and Move Operations
        *************************************************************************************************************/

        Game(const Game &) = delete; // Copy constructor
        Game &operator=(const Game &) = delete; // Copy assignment
        Game(Game &&) = delete; // Move constructor
        Game &operator=(Game &&) = delete; // Move assignment

        /**************************************************************************************************************
        * Getters
        *************************************************************************************************************/

        /**
         * @brief Gets the game board.
         * @return A constant reference to the game board.
         */
        [[nodiscard]] const Board &getBoard() const;

        /**
         * @brief Gets the current player.
         * @return A constant reference to the current player.
         */
        [[nodiscard]] const Player &getCurrentPlayer() const;

        /**
         * @brief Gets the turn number.
         * @return The current turn number.
         */
        [[nodiscard]] size_t getTurnNumber() const { return turnNumber; }

        /**
         * @brief Gets the player by index.
         * @param index The index of the player (0 or 1).
         * @return A constant reference to the requested player.
         */
        [[nodiscard]] const Player &getPlayer(size_t index) const;

        /**************************************************************************************************************
         * Game Logic
         *************************************************************************************************************/

        /**
         * @brief Gets the current game status.
         * @return A `GameStatus` structure indicating if the game is over and the winner.
         */
        [[nodiscard]] GameStatus getGameStatus() const;

        /**
         * @brief Executes a move for the current player.
         *
         * This method validates the move against the game rules, executes it on the game board,
         * updates the undo and redo stacks, and advances the game to the next turn if the move is valid.
         * If the move is invalid, an exception is thrown.
         *
         * @param move The move to be executed.
         * @throw std::runtime_error If the move is invalid or violates game rules.
         *
         * The following operations are performed:
         * - The move is validated using `GameRules::validateMove`.
         * - If valid, the move is executed on the board using `Move::execute`.
         * - The move is pushed onto the undo stack.
         * - The redo stack is cleared after a new move.
         * - The turn is advanced to the next player.
         */
        void executeMove(const Move &move);

        /**
         * @brief Undoes the last move made in the game.
         *
         * This method reverses the most recent move performed in the game. The undone move
         * is transferred from the undo stack to the redo stack. The game state, including
         * the board, current player, and turn number, is updated accordingly.
         *
         * @throw std::runtime_error If the undo stack is empty or the undo operation fails.
         *
         * Steps performed:
         * - Retrieves and removes the last move from the undo stack.
         * - Calls `Move::undo` to reverse the move on the board.
         * - Pushes the move onto the redo stack.
         * - Updates the current player and decrements the turn number.
         */
        void undo();

        /**
          * @brief Redoes the last undone move in the game.
          *
          * This method re-applies the most recently undone move. The redone move
          * is transferred from the redo stack back to the undo stack. The game state,
          * including the board, current player, and turn number, is updated accordingly.
          *
          * @throw std::runtime_error If the redo stack is empty or the redo operation fails.
          *
          * Steps performed:
          * - Retrieves and removes the last move from the redo stack.
          * - Calls `Move::execute` to reapply the move on the board.
          * - Pushes the move onto the undo stack.
          * - Advances the game to the next turn.
          */
        void redo();

        /**
         * @brief Advances the game to the next turn.
         *
         * This method increments the turn number and switches the current player to the next player.
         */
        void nextTurn();

        /**
         * @brief Converts the current state of the game to a string.
         *
         * Provides a detailed representation of the game state, including:
         * - Turn number.
         * - Current player.
         * - Board state.
         * - Game status (ongoing, draw, or winner).
         * - Player information.
         *
         * @return A string describing the current game state.
         */
        [[nodiscard]] std::string toString() const;

        /**
         * @brief Overloads the stream insertion operator for the Game class.
         *
         * Outputs a human-readable string representation of the game state.
         *
         * @param os The output stream.
         * @param game The Game object to output.
         * @return A reference to the output stream.
         */
        friend std::ostream &operator<<(std::ostream &os, const Game &game);
    };
} // namespace hive::models

#endif // GAME_H

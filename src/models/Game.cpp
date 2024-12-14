#include "models/Game.h"

#include <sstream>
#include <stdexcept>
#include <models/GameRules.h>

namespace hive::models {
    void Game::resetGame(const std::string &player1Name, const std::string &player2Name) {
        board->clear();
        undoStack = {};
        redoStack = {};
        turnNumber = 1;
        currentPlayerIndex = 0;
        players[0] = std::make_shared<Player>(1, player1Name);
        players[1] = std::make_shared<Player>(2, player2Name);
    }

    const Board &Game::getBoard() const {
        if (board == nullptr) {
            throw std::runtime_error("Game board is not initialized.");
        }
        return *board;
    }

    Board &Game::getBoard() {
        if (board == nullptr) {
            throw std::runtime_error("Game board is not initialized.");
        }
        return *board;
    }

    const Player &Game::getCurrentPlayer() const {
        if (players[currentPlayerIndex] == nullptr) {
            throw std::runtime_error("Current player is not initialized.");
        }
        return *players[currentPlayerIndex];
    }

    Player &Game::getCurrentPlayer() {
        if (players[currentPlayerIndex] == nullptr) {
            throw std::runtime_error("Current player is not initialized.");
        }
        return *players[currentPlayerIndex];
    }

    const Player &Game::getPlayer(const size_t index) const {
        if (index >= players.size()) {
            throw std::out_of_range("Invalid player index.");
        }
        return *players[index];
    }

    Player &Game::getPlayer(const size_t index) {
        if (index >= players.size()) {
            throw std::out_of_range("Invalid player index.");
        }
        return *players[index];
    }

    std::shared_ptr<Player> Game::getPlayerPtr(const size_t index) {
        if (index == std::numeric_limits<size_t>::max()) {
            // If no index is provided, return the current player
            return players[currentPlayerIndex];
        }

        // Ensure the index is valid (0 or 1)
        if (index < players.size()) {
            return players[index];
        }

        throw std::out_of_range("Invalid player index");
    }

    GameStatus Game::getGameStatus() const {
        const auto result = GameRules::getVictoryCondition(*board,
                                                           {&getPlayer(0), &getPlayer(1)});

        if (result == std::nullopt) {
            return {true, true, nullptr};
        }
        if (result.value() == nullptr) {
            return {false, false, nullptr};
        }
        return {true, false, result.value()};
    }

    void Game::executeMove(const Move &move) {
        try {
            // Validate the move using GameRules
            GameRules::validateMove(move, *board, turnNumber);

            // Execute the move on the board
            move.execute(*board);

            // Push the move onto the undo stack
            undoStack.push(std::make_unique<Move>(move));

            // Clear the redo stack after a new move
            while (!redoStack.empty()) {
                redoStack.pop();
            }

            // Advance to the next turn
            nextTurn();
        } catch (const std::exception &e) {
            // Throw an exception if the move is invalid
            throw std::runtime_error("Invalid move: " + std::string(e.what()));
        }
    }

    void Game::undo() {
        if (undoStack.empty()) {
            throw std::runtime_error("Undo operation failed: undo stack is empty.");
        }

        try {
            auto move = std::move(undoStack.top());
            undoStack.pop();

            move->undo(*board);
            redoStack.push(std::move(move));

            currentPlayerIndex = currentPlayerIndex == 0 ? 1 : 0;
            --turnNumber;
        } catch (const std::exception &e) {
            throw std::runtime_error("Undo operation failed: " + std::string(e.what()));
        }
    }

    void Game::redo() {
        if (redoStack.empty()) {
            throw std::runtime_error("Redo operation failed: redo stack is empty.");
        }

        try {
            auto move = std::move(redoStack.top());
            redoStack.pop();

            move->execute(*board);
            undoStack.push(std::move(move));

            nextTurn();
        } catch (const std::exception &e) {
            throw std::runtime_error("Redo operation failed: " + std::string(e.what()));
        }
    }

    void Game::nextTurn() {
        // Switch to the next player
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
        ++turnNumber;
    }

    std::string Game::toString() const {
        std::ostringstream oss;

        // General game state
        oss << "Game State:\n";
        oss << "Turn Number: " << turnNumber << "\n";
        oss << "Current Player: " << getCurrentPlayer().getName() << "\n";

        // Game status
        if (const auto [isGameOver, isDraw, winner] = getGameStatus(); isGameOver) {
            if (isDraw) {
                oss << "Game Status: Draw\n";
            } else {
                oss << "Game Status: Winner: " << winner->getName() << "\n";
            }
        } else {
            oss << "Game Status: Ongoing\n";
        }

        // Board state
        oss << "Board:\n" << *board << "\n";

        // Player details
        oss << "Players:\n";
        for (size_t i = 0; i < players.size(); ++i) {
            oss << "Player " << i + 1 << ": " << players[i]->getName() << " (ID: " << players[i]->getId() <<
                    ")\n";
        }

        return oss.str();
    }

    std::ostream &operator<<(std::ostream &os, const Game &game) {
        os << game.toString();
        return os;
    }
} // namespace hive::models

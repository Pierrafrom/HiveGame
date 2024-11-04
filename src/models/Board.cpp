#include "models/Board.h"

#include <iostream>

#include "models/enums/Direction.h"
#include <stdexcept>
#include <ranges>
#include <bits/ranges_algo.h>

namespace hive::models {
    // Generate hexes surrounding the given hex location if they don’t already exist
    void Board::_generateSurroundingHexes(const Hex &hex) {
        if (!board.contains(hex)) {
            throw std::invalid_argument("Hex location does not exist");
        }
        for (const auto &direction: {
                 enums::Direction::NORTH_EAST,
                 enums::Direction::EAST,
                 enums::Direction::SOUTH_EAST,
                 enums::Direction::SOUTH_WEST,
                 enums::Direction::WEST,
                 enums::Direction::NORTH_WEST
             }) {
            auto [dx, dy, dz] = getDirectionOffset(direction);

            if (Hex neighborHex(hex.getX() + dx, hex.getY() + dy, hex.getZ() + dz); !board.contains(neighborHex)) {
                board[neighborHex];
            }
        }
    }

    void Board::_freeSurroundingHexes(const Hex &hex) {
        if (!board.contains(hex)) {
            throw std::invalid_argument("Hex location does not exist");
        }
        for (const auto &direction: {
                 enums::Direction::NORTH_EAST,
                 enums::Direction::EAST,
                 enums::Direction::SOUTH_EAST,
                 enums::Direction::SOUTH_WEST,
                 enums::Direction::WEST,
                 enums::Direction::NORTH_WEST
             }) {
            auto [dx, dy, dz] = getDirectionOffset(direction);

            // Check if the neighboring hex exists
            if (Hex neighborHex(hex.getX() + dx, hex.getY() + dy, hex.getZ() + dz); !board.contains(neighborHex)) {
                // If the neighboring hex is empty and has no non-empty neighbors, remove it
                if (board.at(neighborHex).empty() && !_hasNonEmptyNeighbors(neighborHex)) {
                    board.erase(neighborHex);
                }
            }
        }
    }

    // Check if a hex location has non-empty neighbors
    bool Board::_hasNonEmptyNeighbors(const Hex &hex) const {
        if (!board.contains(hex)) {
            throw std::invalid_argument("Hex location does not exist");
        }
        return std::ranges::any_of(getNeighborHexes(hex), [this](const Hex &neighborHex) {
            return isOccupied(neighborHex);
        });
    }

    // Get the singleton instance of the Board
    Board &Board::getInstance() {
        static Board instance;
        return instance;
    }

    // Add a piece to a specific hex location
    void Board::addPiece(const Hex &hex, Piece *piece) {
        board[hex].push(piece);
        _generateSurroundingHexes(hex);
    }

    // Unstack a piece from a specific hex location and return it
    Piece *Board::unstackPiece(const Hex &hex) {
        if (!board.contains(hex)) {
            throw std::invalid_argument("Hex location does not exist");
        }
        if (board.at(hex).empty()) {
            throw std::invalid_argument("Hex location is empty");
        }
        const auto piece = board.at(hex).top();
        board.at(hex).pop();
        if (board.at(hex).empty()) {
            std::cout << "Hex location is empty" << std::endl;
            _freeSurroundingHexes(hex);
        }
        return piece;
    }

    // Check if a hex location is occupied
    bool Board::isOccupied(const Hex &hex) const {
        if (!board.contains(hex)) {
            throw std::invalid_argument("Hex location does not exist");
        }
        return !board.at(hex).empty();
    }

    // Get the top piece at a specific hex location
    Piece *Board::getTopPiece(const Hex &hex) const {
        if (!board.contains(hex)) {
            throw std::invalid_argument("Hex location does not exist");
        }
        if (board.at(hex).empty()) {
            throw std::invalid_argument("Hex location is empty");
        }
        return board.at(hex).top();
    }

    // Get all hexes surrounding a specific hex location
    std::vector<Hex> Board::getNeighborHexes(const Hex &hex) const {
        // Check if the hex location exists
        if (!board.contains(hex)) {
            throw std::invalid_argument("Hex location does not exist");
        }

        // Vector to store neighboring hexes
        std::vector<Hex> neighbors;

        // Iterate over each direction and calculate neighboring hexes
        for (const auto &direction: {
                 enums::Direction::NORTH_EAST,
                 enums::Direction::EAST,
                 enums::Direction::SOUTH_EAST,
                 enums::Direction::SOUTH_WEST,
                 enums::Direction::WEST,
                 enums::Direction::NORTH_WEST
             }) {
            auto [dx, dy, dz] = getDirectionOffset(direction);

            // Calculate the neighboring hex location and check if it exists
            if (Hex neighborHex(hex.getX() + dx, hex.getY() + dy, hex.getZ() + dz); !board.contains(neighborHex)) {
                neighbors.push_back(neighborHex);
            }
        }
        return neighbors;
    }

    // Get the total number of pieces on the board
    size_t Board::pieceCount() const {
        size_t count = 0;

        // Use views::values to directly iterate over the stacks in the board map
        for (const auto &stack: board | std::views::values) {
            count += stack.size(); // Add the number of pieces in each stack
        }

        return count;
    }

    // Check if all pieces on the board are connected
    bool Board::areAllPiecesConnected() const {
        // TODO: Implement this method
    }

    // Clear the board
    void Board::clear() {
        board.clear();
    }
} // namespace hive::models

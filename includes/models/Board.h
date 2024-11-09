#ifndef BOARD_H
#define BOARD_H

#include <unordered_map>
#include <stack>
#include <memory_resource>
#include "Hex.h"
#include "Piece.h"

namespace hive::models {
    class Board {
        // The board is represented as a map of hex locations to a stack of pieces
        std::pmr::unordered_map<Hex, std::stack<Piece *> > board{
            std::pmr::get_default_resource()
        };

        // Private constructor to enforce singleton pattern
        Board() = default;

        // Generate hexes surrounding a given hex location
        void _generateSurroundingHexes(const Hex &hex);

        // Free hexes surrounding a given hex location if no longer needed
        void _freeSurroundingHexes(const Hex &hex);

        // check if a hex location has non-empty neighbors
        bool _hasNonEmptyNeighbors(const Hex &hex) const;

    public:
        /**************************************************************************
         * Singleton pattern
         *************************************************************************/
        static Board &getInstance();

        // Delete copy constructor and assignment operator to enforce singleton
        // Board(const Board &) = delete;

        Board &operator=(const Board &) = delete;

        /**************************************************************************
         * Board methods
         *************************************************************************/

        // Get the board map
        [[nodiscard]]
        const std::pmr::unordered_map<Hex, std::stack<Piece *> > &getBoard() const {
            return board;
        }

        // Add a piece to a specific hex location
        void addPiece(const Hex &hex, Piece *piece);

        // Unstack (remove and return) the top piece from a specific hex location
        Piece *unstackPiece(const Hex &hex);

        // Check if a hex location is occupied
        bool isOccupied(const Hex &hex) const;

        // Get the top piece at a specific hex location
        Piece *getTopPiece(const Hex &hex) const;

        // Get all hexes surrounding a specific hex location
        std::vector<Hex> getNeighborHexes(const Hex &hex) const;

        // Get the total number of pieces on the board
        size_t pieceCount() const;

        // Check if all pieces on the board are connected
        bool areAllPiecesConnected() const;

        // Clear the board (useful for resetting the game)
        void clear();

        // Destructor
        ~Board() = default;
    };
} // namespace hive::models

#endif // BOARD_H

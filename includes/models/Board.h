#ifndef BOARD_H
#define BOARD_H

#include <memory_resource>
#include <unordered_map>
#include <stack>
#include <vector>
#include <cstddef>
#include <iostream>

#include "Hex.h"
#include "Piece.h"

//TODO: implement the movePiece method
//TODO: implement a method to know if a piece can be moved

namespace hive::models {
    /**
     * @class Board
     * @brief Represents the game board for the Hive game.
     *
     * The Board class manages the hexagonal grid on which the Hive game is played.
     * It handles the placement and removal of pieces, as well as the dynamic creation
     * and deletion of hexes (cells) as the game progresses. The board ensures that
     * there is always a single ring of empty hexes surrounding the cluster of pieces.
     *
     * This class is not a singleton because multiple instances may be created to
     * manage game states for features like undo functionality.
     */
    class Board {
        /**
         * @brief The board is represented as a map of hex locations to a stack of pieces.
         *
         * Each Hex key corresponds to a stack of Piece pointers, representing the pieces
         * stacked at that hex location. Empty hexes are also stored to maintain the
         * surrounding ring of empty cells.
         */
        std::pmr::unordered_map<Hex, std::stack<Piece *> > board{std::pmr::get_default_resource()};

        /**
         * @brief Generates hexes surrounding a given hex location.
         * @param hex The hex location around which to generate neighboring hexes.
         *
         * When a piece is added to the board, this method creates empty neighboring
         * hexes (if they don't already exist) around the specified hex. This ensures
         * that there is always one ring of empty hexes surrounding the cluster of pieces.
         */
        void generateSurroundingHexes(const Hex &hex);

        /**
         * @brief Frees hexes surrounding a given hex location if no longer needed.
         * @param hex The hex location around which to free neighboring hexes.
         *
         * When a piece is removed from the board, this method checks each neighboring
         * hex and removes it if it is empty and has no non-empty neighbors. This maintains
         * the rule of having only one ring of empty hexes around the cluster.
         */
        void freeSurroundingHexes(const Hex &hex);

        /**
         * @brief Checks if a hex location has non-empty neighboring hexes.
         * @param hex The hex location to check.
         * @return True if at least one neighboring hex contains a piece; otherwise, false.
         */
        bool hasNonEmptyNeighbors(const Hex &hex) const;

    public:
        /**
         * @brief Constructs a Board and initializes it with an empty hex at (0, 0, 0).
         */
        Board();

        /**
         * @brief Getter for the board map with read access only.
         * @return A const reference to the board map.
         *
         * Provides read-only access to the board map, allowing external classes to
         * retrieve the current state of the board without modifying it.
         */
        const std::pmr::unordered_map<Hex, std::stack<Piece *> > &getBoard() const;

        /**
         * @brief Adds a piece to a specific hex location on the board.
         * @param hex The hex location where the piece will be added.
         * @param piece A pointer to the Piece object to add.
         *
         * Places the piece onto the specified hex, stacking it if other pieces are
         * already present. It also calls generateSurroundingHexes to update the board.
         */
        void addPiece(const Hex &hex, Piece *piece);

        /**
         * @brief Removes and returns the top piece from a specific hex location.
         * @param hex The hex location from which to remove the piece.
         * @return A pointer to the Piece that was removed, or nullptr if none.
         *
         * Removes the top piece from the stack at the specified hex. If the hex becomes
         * empty, freeSurroundingHexes is called to update the board.
         */
        Piece *unstackPiece(const Hex &hex);

        /**
         * @brief Checks if a hex location is occupied by any pieces.
         * @param hex The hex location to check.
         * @return True if the hex has at least one piece; otherwise, false.
         */
        bool isOccupied(const Hex &hex) const;

        /**
         * @brief Retrieves the top piece at a specific hex location.
         * @param hex The hex location to inspect.
         * @return A pointer to the top Piece at the hex, or nullptr if none.
         */
        Piece *getTopPiece(const Hex &hex) const;

        /**
         * @brief Retrieves all hexes surrounding a specific hex location.
         * @param hex The hex location for which to get neighbors.
         * @return A vector of Hex objects representing neighboring hexes.
         *
         * Uses the Direction enum and getDirectionOffset to calculate neighboring hexes.
         * This method returns both occupied and unoccupied neighboring hexes.
         */
        std::vector<Hex> getNeighborHexes(const Hex &hex) const;

        /**
         * @brief Gets the total number of pieces on the board.
         * @return The total count of pieces present on the board.
         */
        size_t pieceCount() const;

        /**
         * @brief Checks if all pieces on the board are connected.
         * @return True if all pieces form a single connected cluster; otherwise, false.
         *
         * Verifies that the Hive remains intact as per game rules. This method can be
         * used to determine if a move would split the hive, which is not allowed.
         */
        bool areAllPiecesConnected() const;

        /**
         * @brief Clears the board, removing all pieces and hexes.
         *
         * Resets the board to its initial empty state. Useful for starting a new game
         * or resetting after an undo operation.
         */
        void clear();

        /**
         * @brief Overloads the stream insertion operator for Board.
         * @param os The output stream.
         * @param board The Board object to output.
         * @return A reference to the output stream.
         *
         * Allows the Board to be output to streams using the << operator, which can be
         * useful for debugging or logging the board's state.
         */
        friend std::ostream &operator<<(std::ostream &os, const Board &board);

        /**
         * @brief Default destructor.
         */
        ~Board() = default;
    };
} // namespace hive::models


#endif // BOARD_H

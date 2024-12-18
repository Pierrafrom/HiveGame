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
#include "enums/Direction.h"

namespace hive::models {
    /**
     * @class Board
     * @brief Represents the game board for the Hive game.
     *
     * The Board class manages the hexagonal grid on which the Hive game is played.
     * It handles the placement and removal of pieces, as well as the dynamic creation
     * and deletion of hexes (cells) as the game progresses. The board ensures that
     * there is always a single ring of empty hexes surrounding the cluster of pieces.
     */
    class Board {
        /**************************************************************************************************
         * Attributes
         *************************************************************************************************/

        /**
         * @brief The board is represented as a map of hex locations to a stack of pieces.
         *
         * Each Hex key corresponds to a stack of `std::shared_ptr<Piece>`, representing the pieces
         * stacked at that hex location. The ownership of the Piece objects is shared between the
         * Board and the Player class.
         */
        std::pmr::unordered_map<Hex, std::stack<std::shared_ptr<Piece> > > board{std::pmr::get_default_resource()};

        /**************************************************************************************************
         * Private methods
         *************************************************************************************************/

        /**
         * @brief Generates hexes surrounding a given hex location.
         * @param hex The hex location around which to generate neighboring hexes.
         *
         * Ensures there is always one ring of empty hexes surrounding the cluster of pieces.
         */
        void generateSurroundingHexes(const Hex &hex);

        /**
         * @brief Frees hexes surrounding a given hex location if no longer needed.
         * @param hex The hex location around which to free neighboring hexes.
         *
         * Removes empty neighboring hexes if they have no non-empty neighbors.
         */
        void freeSurroundingHexes(const Hex &hex);

        /**
         * @brief Checks if a hex location has non-empty neighboring hexes.
         * @param hex The hex location to check.
         * @return True if at least one neighboring hex contains a piece; otherwise, false.
         */
        [[nodiscard]] bool hasNonEmptyNeighbors(const Hex &hex) const;

    public:
        /**************************************************************************************************
         * Constructors
         *************************************************************************************************/

        /**
         * @brief Constructs a Board and initializes it with an empty hex at (0, 0, 0).
         */
        Board();

        /**************************************************************************************************
         * Getters
         *************************************************************************************************/

        /**
         * @brief Retrieves the board map with read access only.
         * @return A const reference to the board map.
         *
         * Provides read-only access to the board map, allowing external classes to
         * retrieve the current state of the board without modifying it.
         */
        [[nodiscard]] const std::pmr::unordered_map<Hex, std::stack<std::shared_ptr<Piece> > > &getBoard() const {
            return board;
        }

        /**
         * @brief Retrieves the top piece at a specific hex location.
         * @param hex The hex location to inspect.
         * @return A shared pointer to the top Piece at the hex, or nullptr if none.
         */
        [[nodiscard]] std::shared_ptr<Piece> getTopPiece(const Hex &hex) const;

        /**************************************************************************************************
         * Public methods
         *************************************************************************************************/

        const std::pmr::unordered_map<Hex, std::stack<std::shared_ptr<Piece>>> &getAllHexes() const {
            return board;
        }

        /**
         * @brief Adds a piece to a specific hex location on the board.
         * @param hex The hex location where the piece will be added.
         * @param piece Shared pointer to the Piece object to add.
         * @throws std::invalid_argument If the provided piece is null.
         */
        void addPiece(const Hex &hex, const std::shared_ptr<Piece> &piece);

        /**
         * @brief Removes and returns the top piece from a specific hex location.
         * @param hex The hex location from which to remove the piece.
         * @return A shared pointer to the Piece that was removed.
         * @throws std::runtime_error If the hex is empty or the piece is not found.
         */
        std::shared_ptr<Piece> unstackPiece(const Hex &hex);

        /**
         * @brief Checks if a hex location is occupied by any pieces.
         * @param hex The hex location to check.
         * @return True if the hex has at least one piece; otherwise, false.
         */
        [[nodiscard]] bool isOccupied(const Hex &hex) const;

        /**
         * @brief Retrieves all hexes surrounding a specific hex location.
         * @param hex The hex location for which to get neighbors.
         * @return A vector of Hex objects representing neighboring hexes.
         */
        [[nodiscard]] std::vector<Hex> neighbors(const Hex &hex) const;

        /**
         * @brief Retrieves all hexes surrounding a specific hex location that are available.
         * @param hex The hex location for which to get neighbors.
         * @return A vector of Hex objects representing neighboring hexes.
         *
         * Uses the Direction enum and getDirectionOffset to calculate neighboring hexes.
         * This method returns only unoccupied neighboring hexes.
         */
        std::vector<Hex> neighborsNotOccupied(const Hex &hex) const;

        /**
         * @brief Gets the total number of pieces on the board.
         * @return The total count of pieces present on the board.
         */
        [[nodiscard]] size_t pieceCount() const;

        /**
         * @brief Checks if all pieces on the board are connected.
         * @return True if all pieces form a single connected cluster; otherwise, false.
         */
        [[nodiscard]] bool areAllPiecesConnected() const;

        /**
         * @brief Moves a piece from one hex to another.
         * @param from The hex location of the piece to move.
         * @param to The target hex location for the piece.
         * @throws std::runtime_error If the source hex is empty or the move is invalid.
         */
        void movePiece(const Hex &from, const Hex &to);

        /**
         * @brief Retrieves the neighboring hex in the given direction.
         * @param hex The starting hex.
         * @param direction The direction to move towards.
         * @return The neighboring hex in the specified direction.
         * @throws std::out_of_range If the neighboring hex does not exist on the board.
         */
        Hex neighbor(const Hex &hex, enums::Direction direction) const;

        /**
         * @brief Checks if a slice can be made between two hexes.
         * @param from The starting hex.
         * @param direction The direction
         *
         * A slice can be made between two hexes if there is a piece on the starting hex
         * and the neighboring hex in the specified direction is empty.
         */
        bool canSliceBetween(const Hex &from, enums::Direction direction) const;

        /**
        * @brief get all empty hexes on the board
        * @return A vector of Hex objects representing empty hexes.
        */
        std::vector<Hex> allEmptyHexes() const;

        /**
         * @brief Clears the board, removing all pieces and hexes.
         */
        void clear();

        /**************************************************************************************************
         * Operators
         *************************************************************************************************/

        /**
         * @brief Overloads the stream insertion operator for Board.
         * @param os The output stream.
         * @param board The Board object to output.
         * @return A reference to the output stream.
         */
        friend std::ostream &operator<<(std::ostream &os, const Board &board);

        /**************************************************************************************************
         * Destructor
         *************************************************************************************************/

        /**
         * @brief Default destructor.
         */
        ~Board() = default;
    };
} // namespace hive::models

#endif // BOARD_H

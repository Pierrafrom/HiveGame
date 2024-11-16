#include "models/Board.h"
#include "models/enums/Direction.h"
#include <queue>
#include <unordered_set>


namespace hive::models {
    /**************************************************************************************************
     * Constructor
     *************************************************************************************************/

    // Constructor implementation
    Board::Board() {
        // Initialize the board with an empty hex at (0, 0, 0)
        const Hex origin(0, 0, 0);
        board[origin]; // Default-initializes the stack at origin to empty
    }

    /**************************************************************************************************
     * Public methods
     *************************************************************************************************/

    // Getter on the board map with read access only
    const std::pmr::unordered_map<Hex, std::stack<Piece *> > &Board::getBoard() const {
        return board;
    }

    // Adds a piece to a specific hex location on the board
    void Board::addPiece(const Hex &hex, Piece *piece) {
        if (!piece) {
            throw std::invalid_argument("Piece cannot be null.");
        }
        if (const auto it = board.find(hex); it == board.end()) {
            throw std::invalid_argument("Hex does not exist on the board.");
        }
        // Place the piece onto the specified hex, stacking if necessary
        board[hex].push(piece);
        // Update the board by generating surrounding hexes
        generateSurroundingHexes(hex);
    }

    // Removes and returns the top piece from a specific hex location
    Piece *Board::unstackPiece(const Hex &hex) {
        if (const auto it = board.find(hex); it != board.end() && !it->second.empty()) {
            // Remove the top piece from the stack
            Piece *removedPiece = it->second.top();
            it->second.pop();

            // If the stack is now empty, update the board by freeing surrounding hexes
            if (it->second.empty()) {
                freeSurroundingHexes(hex);
            }
            return removedPiece;
        }
        // No piece to remove
        return nullptr;
    }

    // Checks if a hex location is occupied by any pieces
    bool Board::isOccupied(const Hex &hex) const {
        const auto it = board.find(hex);
        return it != board.end() && !it->second.empty();
    }

    // Retrieves the top piece at a specific hex location
    Piece *Board::getTopPiece(const Hex &hex) const {
        if (const auto it = board.find(hex); it != board.end() && !it->second.empty()) {
            return it->second.top();
        }
        // No piece at this hex
        return nullptr;
    }

    // Retrieves all hexes surrounding a specific hex location
    std::vector<Hex> Board::getNeighborHexes(const Hex &hex) const {
        std::vector<Hex> neighbors;
        neighbors.reserve(6); // Reserve space for six neighbors

        for (const auto &direction: enums::getAllDirections()) {
            auto [dx, dy, dz] = enums::getDirectionOffset(direction);
            Hex neighbor(hex.getX() + dx, hex.getY() + dy, hex.getZ() + dz);
            neighbors.push_back(neighbor);
        }
        return neighbors;
    }

    // Gets the total number of pieces on the board
    size_t Board::pieceCount() const {
        size_t count = 0;
        for (const auto &pair: board) {
            count += pair.second.size(); // Summing up the size of each stack
        }
        return count;
    }

    // Checks if all pieces on the board are connected
    bool Board::areAllPiecesConnected() const {
        // Find any occupied hex to start the search
        const auto startIt = std::find_if(board.begin(), board.end(), [](const auto &pair) {
            return !pair.second.empty(); // Check if the stack is non-empty
        });

        // If there are no pieces on the board, consider them connected
        if (startIt == board.end()) {
            return true;
        }

        // Perform BFS to traverse connected pieces
        std::unordered_set<Hex> visited;
        std::queue<Hex> queue;
        queue.push(startIt->first);
        visited.insert(startIt->first);

        while (!queue.empty()) {
            Hex current = queue.front();
            queue.pop();

            // Check all neighbors of the current hex
            for (const auto &neighbor: getNeighborHexes(current)) {
                if (isOccupied(neighbor) && !visited.contains(neighbor)) {
                    queue.push(neighbor);
                    visited.insert(neighbor);
                }
            }
        }

        // Count total occupied hexes
        size_t totalOccupied = 0;
        for (const auto &pair: board) {
            if (!pair.second.empty()) {
                totalOccupied++;
            }
        }

        // Check if all occupied hexes were visited
        return visited.size() == totalOccupied;
    }

    // Moves a piece from one hex to another
    void Board::movePiece(const Hex &from, const Hex &to) {
        // Retrieve the top piece from the 'from' hex
        Piece *movedPiece = this->getTopPiece(from);
        if (!movedPiece) {
            throw std::runtime_error("Cannot move a piece from an empty hex.");
        }

        // Place the piece onto the 'to' hex
        addPiece(to, movedPiece);

        // Remove the piece from the 'from' hex
        // We have to remove the piece after adding it to the new hex to avoid freeing the surrounding hexes
        // before the piece is added to the new hex and raise an exception when trying to add the piece
        unstackPiece(from);
    }

    // Clears the board, removing all pieces and hexes
    void Board::clear() {
        board.clear();
        board[Hex(0, 0, 0)];
    }

    // Overloads the stream insertion operator for Board
    std::ostream &operator<<(std::ostream &os, const Board &board) {
        os << "Board state:\n";
        for (const auto &[hex, stack]: board.board) {
            os << hex << ": ";
            if (stack.empty()) {
                os << "Empty";
            } else {
                os << "Stack size " << stack.size();
            }
            os << '\n';
        }
        return os;
    }

    /**************************************************************************************************
     * Private methods
     *************************************************************************************************/

    // Generates hexes surrounding a given hex location
    void Board::generateSurroundingHexes(const Hex &hex) {
        for (const auto &direction: enums::getAllDirections()) {
            auto [dx, dy, dz] = enums::getDirectionOffset(direction);

            // If the neighbor hex doesn't exist in the board, create it with an empty stack
            if (Hex neighbor(hex.getX() + dx, hex.getY() + dy, hex.getZ() + dz); !board.contains(neighbor)) {
                board[neighbor]; // Default-initialized to an empty stack
            }
        }
    }

    // Frees hexes surrounding a given hex location if no longer needed
    void Board::freeSurroundingHexes(const Hex &hex) {
        for (const auto &direction: enums::getAllDirections()) {
            auto [dx, dy, dz] = enums::getDirectionOffset(direction);
            Hex neighbor(hex.getX() + dx, hex.getY() + dy, hex.getZ() + dz);

            if (auto it = board.find(neighbor); it != board.end() && it->second.empty()) {
                // If the neighbor hex is empty and has no non-empty neighbors, remove it
                if (!hasNonEmptyNeighbors(neighbor)) {
                    board.erase(it);
                }
            }
        }
    }

    // Checks if a hex location has non-empty neighboring hexes
    bool Board::hasNonEmptyNeighbors(const Hex &hex) const {
        for (const auto &direction: enums::getAllDirections()) {
            auto [dx, dy, dz] = enums::getDirectionOffset(direction);
            Hex neighbor(hex.getX() + dx, hex.getY() + dy, hex.getZ() + dz);

            if (auto it = board.find(neighbor); it != board.end() && !it->second.empty()) {
                return true; // Found a non-empty neighbor
            }
        }
        return false; // No non-empty neighbors found
    }
} // namespace hive::models

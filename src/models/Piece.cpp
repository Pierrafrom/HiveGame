#include "models/Piece.h"

#include <models/Board.h>

namespace hive::models {
    size_t Piece::pieceNextId = 1; // Initialize static ID counter

    // Get the neighboring pieces of this piece on the board
    std::vector<Piece *> Piece::getNeighboringPieces(const Board &board) const { // TODO : Test this method
        std::vector<Piece *> neighbors;

        // Parcourez les hexagones voisins
        for (std::vector<Hex> neighborHexes = board.getNeighborHexes(board.getPiecePosition(this)); const Hex &hex:
             neighborHexes) {
            if (board.isOccupied(hex)) {
                if (Piece *neighborPiece = board.getTopPiece(hex); neighborPiece != nullptr) {
                    neighbors.push_back(neighborPiece); // Ajoutez à la liste des voisins
                }
            }
        }

        return neighbors;
    }
}

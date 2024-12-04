#include "models/strategies/GrasshopperMoveStrategy.h"
#include <models/Board.h>
#include <models/enums/Direction.h>
#include <vector>

namespace hive::models::strategies {
    std::vector<Hex> GrasshopperMoveStrategy::getPossibleMoves(const Board &board, const Player &player) const {

        const Hex position = this->getPiece().getPosition().value();
        if (!this->hasPiece()) {
            throw std::runtime_error("Move strategy does not have a piece.");
        }
        if (!this->getPiece().hasOwner()) {
            throw std::runtime_error("Piece does not have an owner.");
        }
        if (this->getPiece().getOwner() != player) {
            // Return empty vector because there is movement available for a piece that does not belong to the player
            return {};
        }
        std::vector<Hex> possibleMoves{};

        for (const auto &direction: enums::getAllDirections()) {

            //On définit une variable de la position après la dernière pièce voisine sautée
            Hex current_position = board.neighbor(position, direction);

            //Est-ce qu'il y a une pièce sur l'hexagone voisin
            bool occupied = board.isOccupied(current_position);

            //la sauterelle se déplace en sautant par-dessus au moins 1 pièce
            if (occupied){

                //On recherche le premier hexagone voisin qui est vide :
                while (occupied){
                    //On va chercher l'hexagone voisin dans la même direction
                    current_position = board.neighbor(current_position, direction);
                    occupied = board.isOccupied(current_position);
                }
                const bool connected = isHiveConnectedAfterMove(board, position, current_position);
                if (connected) {
                    possibleMoves.push_back(current_position);
                }
            }
        }

        return possibleMoves;
    }
} // namespace hive::models::strategies

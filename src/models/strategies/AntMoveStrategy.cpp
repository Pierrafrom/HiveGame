#include "models/strategies/AntMoveStrategy.h"
#include <models/Board.h>
#include <models/enums/Direction.h>
#include <vector>

namespace hive::models::strategies {
    std::vector<Hex> AntMoveStrategy::getPossibleMoves(const Board &board, const Player &player) const {

        if (!validatePieceOwnership(player)) {
            return {};
        }

        //get the current position of the piece
        const Hex position = this->getPiece().getPosition().value();

        //initialisation de vector -> un vector vide
        std::vector<Hex> possibleMoves{};

        // Start the exploration from the current position
        antExploration(position, possibleMoves, board, position);

        return possibleMoves;
    }

    //Fonction qui ajoute les hexagones sur lesquels on peut se déplacer depuis une position au vecteur possibleMoves et qui les retourne
    std::vector<Hex> AntMoveStrategy::addPossibleMove(const Hex &current_position, std::vector<Hex> &possibleMoves, const Board &board, const Hex &initiaPosition) const {
        std::vector<Hex> newPositions{}; // initialisation du vecteur contenant les nouvelles positions ajoutées à : vide

        for (const auto &direction: enums::getAllDirections()) {
            const bool occupied = board.isOccupied(board.neighbor(current_position, direction));
            const bool canSlice = board.canSliceBetween(current_position, direction);
            const bool connected = isHiveConnectedAfterMove(board, initiaPosition, board.neighbor(current_position, direction));


            //check if the hex is already in the list
            bool doublon = false;
            for (const Hex& hex : possibleMoves) {
                if (hex == board.neighbor(current_position, direction)) {
                    doublon = true;
                    break;
                }
            }

            if (!occupied && canSlice && connected && !doublon) {
                possibleMoves.push_back(board.neighbor(current_position, direction));
                newPositions.push_back(board.neighbor(current_position, direction));
            }
        }

        return newPositions;
    }

    void AntMoveStrategy::antExploration(const Hex &current_position, std::vector<Hex> &possibleMoves, const Board &board, const Hex &initiaPosition) const {
        std::vector<Hex> newPositions = addPossibleMove(current_position, possibleMoves, board, initiaPosition);
        if (!newPositions.empty()){
            for (const Hex &position : newPositions) antExploration(position, possibleMoves, board, initiaPosition);
        }


    }


} // namespace hive::models::strategies

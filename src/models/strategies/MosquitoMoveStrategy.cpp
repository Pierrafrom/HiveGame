#include "models/strategies/MosquitoMoveStrategy.h"
#include "models/strategies/BeetleMoveStrategy.h"
#include <models/Board.h>
#include <models/Piece.h>
#include <unordered_set>

namespace hive::models::strategies {
    std::vector<Hex> MosquitoMoveStrategy::getPossibleMoves(const Board &board, const Player &player) const {
        // TODO: Implement Mosquito movement logic by imitating adjacent pieces' movement abilities

        if (!validatePieceOwnership(player)) {
            return {};
        }

        const Hex position = this->getPiece().getPosition().value();

        std::unordered_set<Hex> uniqueMoves;

        auto hashEntry = board.getBoard().find(position);

        //condition sur si la piece est connectée est au dessus d'une pile => scarabé
        //Si on est sur une pile avec plusieurs hexagone et que la piece est au sommet, alors le moustique se déplace comme un scarabé
        if (hashEntry != board.getBoard().end()) {
            const auto &stack = hashEntry->second;

            if (stack.size() > 1 && board.getTopPiece(position).get() == &this->getPiece()) {

                BeetleMoveStrategy beetleStrategy(const_cast<Piece*>(&this->getPiece()));
                const std::vector<Hex> possibleMoves = beetleStrategy.getPossibleMoves(board, player, position);

                return possibleMoves;
            }
        }

        //Sinon elle peut prendre toutes les autres formes
        const std::vector<std::shared_ptr<const Piece> > neighbors = this->getPiece().getNeighbors(board);
        //Problème : is Hive connected after move et can slice between sont appelés sur les voisins et non sur la cette piece même
        for (const auto &neighbor : neighbors) {
            if (neighbor) {
                const auto &neighborMoveStrategy = neighbor->getMoveStrategy();
                const std::vector<Hex> neighborMoves = neighborMoveStrategy.getPossibleMoves(board, player, position);
                uniqueMoves.insert(neighborMoves.begin(), neighborMoves.end());
            }
        }

        std::vector<Hex> possibleMoves(uniqueMoves.begin(), uniqueMoves.end());

        return possibleMoves; // PLACEHOLDER
    }

    std::vector<Hex> MosquitoMoveStrategy::getPossibleMoves(const Board &board, const Player &player, const Hex &hex) const {
        // TODO: Implement Mosquito movement logic by imitating adjacent pieces' movement abilities

        if (!validatePieceOwnership(player)) {
            return {};
        }

        const Hex position = hex;

        std::unordered_set<Hex> uniqueMoves;

        auto hashEntry = board.getBoard().find(position);

        //condition sur si la piece est connectée est au dessus d'une pile => scarabé
        //Si on est sur une pile avec plusieurs hexagone et que la piece est au sommet, alors le moustique se déplace comme un scarabé
        if (hashEntry != board.getBoard().end()) {
            const auto &stack = hashEntry->second;

            if (stack.size() > 1 && board.getTopPiece(position).get() == &this->getPiece()) {

                BeetleMoveStrategy beetleStrategy(const_cast<Piece*>(&this->getPiece()));
                const std::vector<Hex> possibleMoves = beetleStrategy.getPossibleMoves(board, player, position);

                return possibleMoves;
            }
        }

        //Sinon elle peut prendre toutes les autres formes
        const std::vector<std::shared_ptr<const Piece> > neighbors = this->getPiece().getNeighbors(board);
        //Problème : is Hive connected after move et can slice between sont appelés sur les voisins et non sur la cette piece même
        for (const auto &neighbor : neighbors) {
            if (neighbor) {
                const auto &neighborMoveStrategy = neighbor->getMoveStrategy();
                const std::vector<Hex> neighborMoves = neighborMoveStrategy.getPossibleMoves(board, player, position);
                uniqueMoves.insert(neighborMoves.begin(), neighborMoves.end());
            }
        }

        std::vector<Hex> possibleMoves(uniqueMoves.begin(), uniqueMoves.end());

        return possibleMoves; // PLACEHOLDER
    }

} // namespace hive::models::strategies

#include "models/strategies/SpiderMoveStrategy.h"
#include <set>
#include <models/Board.h>
#include "config/constants.h"

namespace hive::models::strategies {
    std::vector<Hex> SpiderMoveStrategy::getPossibleMoves(const Board &board, const Player &player) const {

        if (!validatePieceOwnership(player)) {
            return {};
        }

        //get the current position of the piece
        const Hex position = this->getPiece().getPosition().value();

        std::set<Hex> visited;
        std::set<Hex> reachable;

        // add the current position to visited
        visited.insert(position);

        // Start the exploration from the current position
        spiderExploration(position, position, 0, visited, reachable, board);

        // convert the set to a vector to return
        std::vector<Hex> result(reachable.begin(), reachable.end());

        // erase the current position from the reachable positions

        result.erase(std::remove(result.begin(), result.end(), position), result.end());

        return result;
    }

    std::vector<Hex> SpiderMoveStrategy::getPossibleMoves(const Board &board, const Player &player, const Hex &hex) const {

        if (!validatePieceOwnership(player)) {
            return {};
        }

        //get the current position of the piece
        const Hex position = hex;

        std::set<Hex> visited;
        std::set<Hex> reachable;

        // add the current position to visited
        visited.insert(position);

        // Start the exploration from the current position
        spiderExploration(position, position, 0, visited, reachable, board);

        // convert the set to a vector to return
        std::vector<Hex> result(reachable.begin(), reachable.end());

        // erase the current position from the reachable positions

        result.erase(std::remove(result.begin(), result.end(), position), result.end());

        return result;
    }

    void SpiderMoveStrategy::spiderExploration(const Hex &initialPosition, const Hex &current, int depth, std::set<Hex> &visited, std::set<Hex> &reachable, const Board &board) const {
        if (depth == hive::config::constants::MAX_SPIDER_MOVES) {
            reachable.insert(current);
            return;
        }

        for (const Hex &neighbor : board.neighborsNotOccupied(current)) {
            for (const Hex &neighbor : board.neighborsNotOccupied(current)) {
                const bool connected = isHiveConnectedAfterMove(board, initialPosition, neighbor);

                if (connected) {
                    // check if the neighbor is not already visited
                    if (!visited.contains(neighbor)) {
                        visited.insert(neighbor);
                        spiderExploration(initialPosition,neighbor, depth + 1, visited, reachable, board);
                        visited.erase(neighbor);
                    }
                }
            }
        }
        // namespace hive::models::strategies
    }
}
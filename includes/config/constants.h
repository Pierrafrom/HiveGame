#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "models/enums/PieceType.h"
#include <unordered_map>

namespace hive::config::constants {
    /**
     * @brief Maximum allowed Queen Bee pieces per player.
     */
    constexpr int MAX_QUEEN_BEE = 1;

    /**
     * @brief Maximum allowed Ant pieces per player.
     */
    constexpr int MAX_ANT = 3;

    /**
     * @brief Maximum allowed Beetle pieces per player.
     */
    constexpr int MAX_BEETLE = 2;

    /**
     * @brief Maximum allowed Ladybug pieces per player.
     */
    constexpr int MAX_LADYBUG = 1;

    /**
     * @brief Maximum allowed Mosquito pieces per player.
     */
    constexpr int MAX_MOSQUITO = 1;

    /**
     * @brief Maximum allowed Spider pieces per player.
     */
    constexpr int MAX_SPIDER = 2;

    /**
     * @brief Maximum allowed Pillbug pieces per player.
     */
    constexpr int MAX_PILLBUG = 1;

    /**
     * @brief Maximum allowed Grasshopper pieces per player.
     */
    constexpr int MAX_GRASSHOPPER = 3;

    /**
     * @brief Maximum number of turns before the Queen Bee must be placed.
     */
    constexpr int MAX_TURN_BEFORE_QUEEN_PLACEMENT = 4;

    /**
     * @brief Maximum number of moves a Spider can make.
     */
    constexpr int MAX_SPIDER_MOVES = 3;

    /**
     * @brief Maximum number of pieces for one player.
     */
    constexpr int MAX_PIECES_PER_PLAYER = MAX_ANT + MAX_BEETLE + MAX_GRASSHOPPER + MAX_LADYBUG + MAX_MOSQUITO +
                                          MAX_PILLBUG + MAX_QUEEN_BEE + MAX_SPIDER;

    /**
     * @brief Mapping of each piece type to its maximum allowable count per player.
     *
     * This unordered map provides quick access to the maximum count of each piece type, which
     * is useful for validating if a player has exceeded the allowed quantity of any particular
     * piece during the game.
     */
    const std::unordered_map<models::enums::PieceType, int> MAX_PIECES = {
        {models::enums::PieceType::QUEEN_BEE, MAX_QUEEN_BEE},
        {models::enums::PieceType::ANT, MAX_ANT},
        {models::enums::PieceType::BEETLE, MAX_BEETLE},
        {models::enums::PieceType::LADYBUG, MAX_LADYBUG},
        {models::enums::PieceType::MOSQUITO, MAX_MOSQUITO},
        {models::enums::PieceType::SPIDER, MAX_SPIDER},
        {models::enums::PieceType::PILLBUG, MAX_PILLBUG},
        {models::enums::PieceType::GRASSHOPPER, MAX_GRASSHOPPER}
    };
} // namespace hive::config::constants

#endif // CONSTANTS_H

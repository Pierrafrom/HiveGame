#ifndef DIRECTION_H
#define DIRECTION_H

#include <tuple>
#include <stdexcept>

namespace hive::models::enums {
    enum class Direction {
        NORTH_EAST,
        EAST,
        SOUTH_EAST,
        SOUTH_WEST,
        WEST,
        NORTH_WEST
    };

    // Function to get the offset coordinates for each direction in cubical coordinates
    constexpr std::tuple<int, int, int> getDirectionOffset(const Direction direction) {
        switch (direction) {
            case Direction::NORTH_EAST: return {1, -1, 0};
            case Direction::EAST: return {1, 0, -1};
            case Direction::SOUTH_EAST: return {0, 1, -1};
            case Direction::SOUTH_WEST: return {-1, 1, 0};
            case Direction::WEST: return {-1, 0, 1};
            case Direction::NORTH_WEST: return {0, -1, 1};
            default:
                throw std::invalid_argument("Invalid direction");
        }
    }
} // namespace hive::models::enums

#endif // DIRECTION_H

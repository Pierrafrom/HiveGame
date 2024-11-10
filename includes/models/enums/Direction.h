#ifndef DIRECTION_H
#define DIRECTION_H

#include <tuple>
#include <stdexcept>
#include <array>


namespace hive::models::enums {
    /**
     * @enum Direction
     * @brief Enumerates the six possible directions on a hexagonal grid.
     *
     * Represents the six cardinal directions used to navigate a hexagonal grid
     * using cube coordinates.
     */
    enum class Direction {
        NORTH_EAST, /**< Direction towards North-East */
        EAST, /**< Direction towards East */
        SOUTH_EAST, /**< Direction towards South-East */
        SOUTH_WEST, /**< Direction towards South-West */
        WEST, /**< Direction towards West */
        NORTH_WEST /**< Direction towards North-West */
    };

    /**
     * @brief Retrieves the offset coordinates for each direction in cube coordinates.
     * @param direction The direction for which to get the offset.
     * @return A tuple containing the (x, y, z) offsets for the specified direction.
     *
     * Provides the cube coordinate offsets corresponding to each direction,
     * which can be used to find neighboring hexagons on the grid.
     */
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

    /**
     * @brief Retrieves all possible directions in an array.
     * @return A std::array containing all Direction enum values.
     *
     * Useful for iterating over all directions when examining neighboring hexes.
     */
    constexpr std::array<Direction, 6> getAllDirections() {
        return {
            Direction::NORTH_EAST, Direction::EAST, Direction::SOUTH_EAST,
            Direction::SOUTH_WEST, Direction::WEST, Direction::NORTH_WEST
        };
    }


} // namespace hive::models::enums


#endif // DIRECTION_H

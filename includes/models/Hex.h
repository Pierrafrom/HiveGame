#ifndef HEX_H
#define HEX_H

#include <cstdint>
#include <functional>
#include <ostream>


namespace hive::models {
    /**
     * @class Hex
     * @brief Represents a hexagonal cell in the Hive game board using cube coordinates.
     *
     * The Hex class models a hexagonal grid cell with cube coordinates (x, y, z),
     * where x + y + z must always equal zero. This representation simplifies the
     * computation of neighboring cells and distances within the grid.
     */
    class Hex {
        /***************************************************************************
         * Attributes
         ***************************************************************************/

        const std::int8_t x; /**< X coordinate of the hex cell */
        const std::int8_t y; /**< Y coordinate of the hex cell */
        const std::int8_t z; /**< Z coordinate of the hex cell */

    public:
        /***************************************************************************
         * Constructors
         ***************************************************************************/

        /**
         * @brief Constructs a Hex object with specified cube coordinates.
         * @param x The x-coordinate.
         * @param y The y-coordinate.
         * @param z The z-coordinate. Must satisfy x + y + z == 0.
         *
         * Constructs a Hex with the given coordinates. The coordinates must
         * satisfy the constraint x + y + z == 0; otherwise, the program will
         * terminate with an assertion error.
         */
        Hex(std::int8_t x, std::int8_t y, std::int8_t z);

        /**
         * @brief Default copy constructor for Hex.
         */
        Hex(const Hex &other) = default;

        /**
        * @brief smaller assignment operator for Hex.
        */
        bool operator<(const Hex& other) const;

        /**
         * @brief Default move constructor for Hex.
         */
        Hex(Hex &&) noexcept = default;

        /**
         * @brief Move assignment operator for Hex.
         */
        Hex &operator=(Hex &&) noexcept;

        /***************************************************************************
         * Destructor
         ***************************************************************************/

        /**
         * @brief Default destructor for Hex.
         */
        ~Hex() = default;

        /***************************************************************************
         * Getters
         ***************************************************************************/

        /**
         * @brief Retrieves the x-coordinate of the hex cell.
         * @return The x-coordinate.
         */
        [[nodiscard]] std::int8_t getX() const { return x; }

        /**
         * @brief Retrieves the y-coordinate of the hex cell.
         * @return The y-coordinate.
         */
        [[nodiscard]] std::int8_t getY() const { return y; }

        /**
         * @brief Retrieves the z-coordinate of the hex cell.
         * @return The z-coordinate.
         */
        [[nodiscard]] std::int8_t getZ() const { return z; }

        /***************************************************************************
         * Public methods
         ***************************************************************************/

        /**
         * @brief Generates a hash code for the hex cell.
         * @return A size_t hash code representing the hex cell.
         *
         * Combines the x, y, and z coordinates to produce a unique hash code.
         * Useful for storing Hex objects in hash-based containers like std::unordered_map.
         */
        [[nodiscard]] size_t hash_code() const;

        /**
         * @brief Checks equality between two Hex objects.
         * @param other Another Hex object to compare with.
         * @return True if both Hex objects have the same coordinates; otherwise, false.
         */
        bool operator==(const Hex &other) const;

        /**
         * @brief Checks inequality between two Hex objects.
         * @param other Another Hex object to compare with.
         * @return True if the Hex objects have different coordinates; otherwise, false.
         */
        bool operator!=(const Hex &other) const;

        /**
         * @brief Converts the Hex object to a string representation.
         * @return A string representation of the Hex object.
         *
         * The string representation is formatted as "Hex(x, y, z)".
         */
        [[nodiscard]] std::string toString() const;

        /**
         * @brief Overloads the stream insertion operator for Hex objects.
         * @param os The output stream.
         * @param hex The Hex object to insert into the stream.
         * @return A reference to the output stream.
         *
         * Allows Hex objects to be outputted to streams using the << operator,
         * formatted as "Hex(x, y, z)".
         */
        friend std::ostream &operator<<(std::ostream &os, const Hex &hex);
    };
} // namespace hive::models

/**
 * @brief Hash function specialization for Hex objects.
 *
 * Allows Hex objects to be used as keys in hash-based containers like std::unordered_map.
 */
template<>
struct std::hash<hive::models::Hex> {
    /**
     * @brief Computes the hash code for a Hex object.
     * @param hex The Hex object to hash.
     * @return A size_t hash code.
     */
    size_t operator()(const hive::models::Hex &hex) const noexcept {
        return hex.hash_code();
    }
}; // namespace std

#endif // HEX_H

#include "models/Hex.h"
#include <cassert>


namespace hive::models {
    /*******************************************************************************************************************
     * Constructors
     ******************************************************************************************************************/
    // Constructor implementation
    Hex::Hex(const std::int8_t x, const std::int8_t y, const std::int8_t z) : x(x), y(y), z(z) {
        assert(x + y + z == 0); // Ensure the coordinates satisfy x + y + z == 0
    }

    bool Hex::operator<(const Hex &other) const {
            if (x != other.x) {
                return x < other.x;
            }
            return y < other.y;
    }

    Hex &Hex::operator=(Hex &&other) noexcept {
        if (this != &other) {
            // Use const_cast to modify the const members
            const_cast<std::int8_t &>(x) = other.x;
            const_cast<std::int8_t &>(y) = other.y;
            const_cast<std::int8_t &>(z) = other.z;
        }
        return *this;
    }

    /*******************************************************************************************************************
     * Public methods
     ******************************************************************************************************************/

    // Generates a hash code for the Hex object
    size_t Hex::hash_code() const {
        // Combines the hash codes of x, y, and z coordinates
        return std::hash<int>{}(x) ^ std::hash<int>{}(y) << 1 ^ std::hash<int>{}(z) << 2;
    }

    // String representation of the Hex object
    std::string Hex::toString() const {
        return "Hex(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }

    /*******************************************************************************************************************
     * Operators
     ******************************************************************************************************************/

    // Equality operator implementation
    bool Hex::operator==(const Hex &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    // Inequality operator implementation
    bool Hex::operator!=(const Hex &other) const {
        return !(*this == other);
    }

    // Stream insertion operator implementation
    std::ostream &operator<<(std::ostream &os, const Hex &hex) {
        os << hex.toString();
        return os;
    }

} // namespace hive::models

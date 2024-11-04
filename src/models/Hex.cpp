#include "models/Hex.h"

#include <cassert>

namespace hive::models {
    // Constructor
    Hex::Hex(const std::int8_t x, const std::int8_t y, const std::int8_t z) : x(x), y(y), z(z) {
        assert(x + y + z == 0);
    }

    // Hash code function to create a unique hash for each Hex
    size_t Hex::hash_code() const {
        return std::hash<int>{}(x) ^ (std::hash<int>{}(y) << 1) ^ (std::hash<int>{}(z) << 2);
    }

    // Equality operator
    bool Hex::operator==(const Hex &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    // Inequality operator
    bool Hex::operator!=(const Hex &other) const {
        return !(*this == other);
    }

    // Overload << operator for Hex
    std::ostream &operator<<(std::ostream &os, const Hex &hex) {
        os << "Hex(" << static_cast<int>(hex.getX()) << ", " << static_cast<int>(hex.getY()) << ", "
                << static_cast<int>(hex.getZ()) << ")";
        return os;
    }
} // namespace hive::models

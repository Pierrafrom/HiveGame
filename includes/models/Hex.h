#ifndef HEX_H
#define HEX_H

#include <cstdint>
#include <functional>
#include <ostream>

namespace hive::models {
    class Hex {
        const std::int8_t x;
        const std::int8_t y;
        const std::int8_t z;

    public:
        Hex(std::int8_t x, std::int8_t y, std::int8_t z);

        [[nodiscard]] std::int8_t getX() const { return x; }
        [[nodiscard]] std::int8_t getY() const { return y; }
        [[nodiscard]] std::int8_t getZ() const { return z; }

        [[nodiscard]] size_t hash_code() const;

        bool operator==(const Hex &other) const;

        bool operator!=(const Hex &other) const;

        ~Hex() = default;
    };

    // Overload the << operator for Hex
    std::ostream &operator<<(std::ostream &os, const Hex &hex);
} // namespace hive::models

// Specialization of std::hash for hive::models::Hex
template<>
struct std::hash<hive::models::Hex> {
    size_t operator()(const hive::models::Hex &hex) const noexcept {
        return hex.hash_code();
    }
};

#endif // HEX_H

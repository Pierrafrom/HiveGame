#ifndef PIECE_TYPE_H
#define PIECE_TYPE_H

#include <string>

namespace hive::models::enums {
    /**
    * @enum PieceType
    * @brief Enumerates the different types of pieces in the Hive game.
    */
    enum class PieceType {
        QUEEN_BEE,
        ANT,
        BEETLE,
        LADYBUG,
        MOSQUITO,
        SPIDER,
        PILLBUG,
        GRASSHOPPER
    };

    /**
     * @brief Converts a PieceType enum to its string representation.
     * @param type The PieceType enum value.
     * @return A string representing the PieceType.
     */
    inline std::string toString(const PieceType type) {
        switch (type) {
            case PieceType::QUEEN_BEE: return "Queen Bee";
            case PieceType::ANT: return "Ant";
            case PieceType::BEETLE: return "Beetle";
            case PieceType::LADYBUG: return "Ladybug";
            case PieceType::MOSQUITO: return "Mosquito";
            case PieceType::SPIDER: return "Spider";
            case PieceType::PILLBUG: return "Pillbug";
            case PieceType::GRASSHOPPER: return "Grasshopper";
            default: return "Unknown";
        }
    }

    /**
     * @brief Overloads the << operator to output the string representation of a PieceType.
     * @param os The output stream.
     * @param type The PieceType enum value.
     * @return The output stream with the PieceType string representation.
     */
    inline std::ostream &operator<<(std::ostream &os, const PieceType type) {
        return os << toString(type);
    }
} // namespace hive::models::enums

#endif // PIECE_TYPE_H

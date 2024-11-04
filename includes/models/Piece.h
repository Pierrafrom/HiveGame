#ifndef PIECE_H
#define PIECE_H

#include <cstddef> // For size_t

namespace hive::models {
    class Piece {
        size_t id;

    public:
        // Constructor to initialize a Piece with an ID
        explicit Piece(const size_t id) : id(id) {
        }

        // Getter for the ID
        [[nodiscard]] size_t getId() const { return id; }
    };
} // namespace hive::models

#endif // PIECE_H

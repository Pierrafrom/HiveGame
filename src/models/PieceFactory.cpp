#include "models/PieceFactory.h"

#include <models/pieces/Ant.h>
#include <models/pieces/Beetle.h>
#include <models/pieces/Grasshopper.h>
#include <models/pieces/Ladybug.h>
#include <models/pieces/Mosquito.h>
#include <models/pieces/Pillbug.h>
#include <models/pieces/QueenBee.h>
#include <models/pieces/Spider.h>

namespace hive::models {
    // Factory method to create a piece based on the specified type
    std::unique_ptr<Piece> PieceFactory::createPiece(const enums::PieceType type, size_t id) {
        switch (type) {
            case enums::PieceType::QUEEN_BEE:
                return id
                           ? std::make_unique<pieces::QueenBee>(id)
                           : std::make_unique<pieces::QueenBee>();
            case enums::PieceType::ANT:
                return id
                           ? std::make_unique<pieces::Ant>(id)
                           : std::make_unique<pieces::Ant>();
            case enums::PieceType::BEETLE:
                return id
                           ? std::make_unique<pieces::Beetle>(id)
                           : std::make_unique<pieces::Beetle>();
            case enums::PieceType::GRASSHOPPER:
                return id
                           ? std::make_unique<pieces::Grasshopper>(id)
                           : std::make_unique<pieces::Grasshopper>();
            case enums::PieceType::LADYBUG:
                return id
                           ? std::make_unique<pieces::Ladybug>(id)
                           : std::make_unique<pieces::Ladybug>();
            case enums::PieceType::MOSQUITO:
                return id
                           ? std::make_unique<pieces::Mosquito>(id)
                           : std::make_unique<pieces::Mosquito>();
            case enums::PieceType::PILLBUG:
                return id
                           ? std::make_unique<pieces::Pillbug>(id)
                           : std::make_unique<pieces::Pillbug>();
            case enums::PieceType::SPIDER:
                return id
                           ? std::make_unique<pieces::Spider>(id)
                           : std::make_unique<pieces::Spider>();
            default:
                throw std::invalid_argument("Invalid piece type.");
        }
    }
} // namespace hive::models

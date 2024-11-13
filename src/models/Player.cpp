#include "models/Player.h"

namespace hive::models {
    size_t Player::nextId = 0;
    void Player::addPiece(std::unique_ptr<Piece> piece) {
        pieces.push_back(std::move(piece));
    }

    int Player::getPieceCount(const enums::PieceType type) const {
        int count = 0;
        for(const auto& piece : pieces) {
            if(piece->getType() == type) {
                count++;
            }
        }
        return count;
    }
    


}

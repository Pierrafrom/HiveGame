#ifndef HIVEGAME_PIECEFACTORY_H
#define HIVEGAME_PIECEFACTORY_H

#include "Piece.h"
#include "models/enums/PieceType.h"
#include "Ant.h"
#include "Beetle.h"
#include "Grasshopper.h"
#include "LadyBug.h"
#include "Mosquito.h"
#include "Pillbug.h"
#include "QueenBee.h"
#include "Spider.h"

namespace hive::models{
    /**
     * Design pattern factory pour instancier les objets filles de la classe piece selon le bon type passé en paramètre
     *
     * Reste à faire :
     * • Vérifier que le nombre de pièce max n'a pas été atteint
     */
    class PieceFactory {
    public:
        std::unique_ptr<Piece> createPiece(enums::PieceType type, const size_t id, const Player* player, const Hex& posInitiale);
        std::unique_ptr<Piece> createPiece(enums::PieceType type, const size_t id);
    };
}

#endif //HIVEGAME_PIECEFACTORY_H

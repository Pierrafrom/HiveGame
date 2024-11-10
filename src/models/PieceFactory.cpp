#include "models/PieceFactory.h"

namespace hive::models{

    /**
     * Méthode qui instancie l'objet dérivé de Piece selon le type souhaité
     *
     * Reste à faire :
     * • Vérifier le nombre max de piece par type !
     *
     */

    std::unique_ptr<Piece> createPiece(enums::PieceType type, const size_t id, const Player* player, const Hex& posInitiale) { //mettre auto au lieu de Piece* ?
        switch (type) {
            case enums::PieceType::Ant:
                //Condition pour savoir si le joueur a créé le nombre max de Ant --> à fixer en fonction de la classe player
                return std::make_unique<Ant>(id, player, posInitiale);

            case enums::PieceType::Grasshopper:
                return std::make_unique<Grasshopper>(id, player, posInitiale);

            case enums::PieceType::QueenBee:
                return std::make_unique<QueenBee>(id, player, posInitiale);

            case enums::PieceType::Beetle:
                return std::make_unique<Beetle>(id, player, posInitiale);

            case enums::PieceType::Ladybug:
                return std::make_unique<LadyBug>(id, player, posInitiale);

            case enums::PieceType::Mosquito:
                return std::make_unique<Mosquito>(id, player, posInitiale);

            case enums::PieceType::Spider:
                return std::make_unique<Spider>(id, player, posInitiale);

            case enums::PieceType::Pillbug:
                return std::make_unique<Pillbug>(id, player, posInitiale);

            default:
                return nullptr;
        }
    }


}

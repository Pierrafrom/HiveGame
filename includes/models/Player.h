#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <memory>
#include "models/Piece.h"

namespace hive::models {
    /**
     * @class Player
     * @brief Représente un joueur dans le jeu Hive.
     *
     * Gère les pièces du joueur et permet l'interaction avec la logique du jeu.
     */
    class Player {
    private:
        size_t id; /**< Identifiant unique du joueur */
        std::vector<std::unique_ptr<Piece>> pieces; /**< Pièces possédées par le joueur */

    public:
        /**
         * @brief Constructeur de la classe Player.
         * @param id Identifiant unique du joueur.
         */
        Player(size_t id);

        /**
         * @brief Destructeur.
         */
        ~Player() = default;

        /**
         * @brief Ajoute une pièce à la collection du joueur.
         * @param piece Pointeur unique vers la pièce à ajouter.
         */
        void addPiece(std::unique_ptr<Piece> piece);

        /**
         * @brief Obtient les pièces du joueur.
         * @return Référence au vecteur des pièces.
         */
        std::vector<std::unique_ptr<Piece>>& getPieces();

        /**
         * @brief Obtient l'identifiant du joueur.
         * @return L'identifiant du joueur.
         */
        [[nodiscard]] size_t getId() const;
    };
} // namespace hive::models

#endif // PLAYER_H

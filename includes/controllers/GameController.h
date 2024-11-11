#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "models/Game.h"

// TODO: implement the GameController class (not a priority at the moment)

namespace hive::controllers {
    /**
     * @class GameController
     * @brief Contrôle la logique de haut niveau du jeu, interagit avec la vue et le modèle.
     *
     * La classe GameController gère les interactions entre l'utilisateur, la vue et le modèle,
     * en orchestrant les opérations nécessaires pour jouer une partie.
     */
    class GameController {
    private:
        models::Game game; /**< La partie en cours */

    public:
        /**
         * @brief Construit un GameController avec les joueurs donnés.
         * @param player1 Le premier joueur.
         * @param player2 Le second joueur.
         */
        GameController(const models::Player &player1, const models::Player &player2);

        /**
         * @brief Démarre la boucle principale du jeu.
         */
        void startGame();

        /**
         * @brief Gère l'entrée utilisateur pour effectuer un mouvement.
         */
        void handlePlayerMove();

        /**
         * @brief Destructeur.
         */
        ~GameController() = default;
    };
} // namespace hive::controllers

#endif // GAMECONTROLLER_H

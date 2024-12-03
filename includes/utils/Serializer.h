#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "models/Game.h"
#include <string>

// TODO: Implement the Serializer class (not a priority at the moment)

namespace hive::utils {
    /**
     * @class Serializer
     * @brief Fournit des méthodes pour sérialiser et désérialiser l'état du jeu.
     *
     * La classe Serializer permet de sauvegarder et de charger l'état du jeu, afin de pouvoir reprendre une partie ultérieurement.
     */
    class Serializer {
    public:
        /**
         * @brief Sauvegarde l'état du jeu dans un fichier.
         * @param game La partie à sauvegarder.
         * @param filename Le nom du fichier où sauvegarder.
         * @return True si la partie a été sauvegardée avec succès, sinon false.
         */
        static bool saveGame(const models::Game &game, const std::string &filename);

        /**
         * @brief Charge une partie à partir d'un fichier.
         * @param game L'objet Game dans lequel charger la partie.
         * @param filename Le nom du fichier à charger.
         * @return True si la partie a été chargée avec succès, sinon false.
         */
        static bool loadGame(models::Game &game, const std::string &filename);
    };
} // namespace hive::utils

#endif // SERIALIZER_H

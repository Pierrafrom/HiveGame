#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "models/Game.h"
#include <string>

namespace hive::utils {
    /**
     * @class Serializer
     * @brief Provides methods for serializing and deserializing the state of the game.
     *
     * The Serializer class allows saving and loading the game state to/from a file.
     */
    class Serializer {
    public:
        /**
         * @brief Saves the current game state to a file.
         * @param game The game instance to save.
         * @param filename The name of the file to save to.
         * @return True if the game was successfully saved, otherwise false.
         */
        static bool saveGame(const hive::models::Game &game, const std::string &filename);

        /**
         * @brief Loads a game state from a file.
         * @param game The game instance to load into.
         * @param filename The name of the file to load from.
         * @return True if the game was successfully loaded, otherwise false.
         */
        static bool loadGame(hive::models::Game &game, const std::string &filename);
    };
} // namespace hive::utils

#endif // SERIALIZER_H

#include <gtest/gtest.h>
#include "models/Game.h"
#include "utils/Serializer.h"
#include <nlohmann/json.hpp>
#include <fstream>

namespace hive::utils {
    /**
     * @brief Test fixture class for Serializer unit tests.
     */
    class SerializerTest : public testing::Test {
    protected:
        hive::models::Game &game = hive::models::Game::getInstance(); /**< Singleton Game instance */
        hive::utils::Serializer &serializer = hive::utils::Serializer::getInstance(); /**< Singleton Serializer instance */
        std::string testFilename = "test_game_state.json"; /**< Test file name */

        void SetUp() override {
            // Reset game state before each test
            game.resetGame("Alice", "Bob");
        }

        void TearDown() override {
            // Clean up test file after each test
            std::remove(testFilename.c_str());
        }
    };

    /*TEST_F(SerializerTest, SaveGame) {
        // Act : Sauvegarder l'état initial du jeu
        bool saveResult = Serializer::saveGame(game, testFilename);

        // Assert : Vérifie que la sauvegarde a réussi
        EXPECT_TRUE(saveResult); // La fonction doit retourner true

        // Vérifie que le fichier existe et peut être ouvert
        std::ifstream file(testFilename);
        ASSERT_TRUE(file.is_open()) << "Le fichier n'a pas été créé.";

        // Charge le contenu du fichier dans un objet JSON
        nlohmann::json jsonGame;
        EXPECT_NO_THROW(file >> jsonGame);
        file.close();

        // Vérifie que les clés principales existent dans le JSON
        EXPECT_TRUE(jsonGame.contains("players"));
        EXPECT_TRUE(jsonGame.contains("undoStack"));
        EXPECT_TRUE(jsonGame.contains("redoStack"));

        // Vérifie les informations des joueurs
        ASSERT_EQ(jsonGame["players"].size(), 2);
        EXPECT_EQ(jsonGame["players"][0]["name"], "Alice");
        EXPECT_EQ(jsonGame["players"][1]["name"], "Bob");

        // Vérifie que les piles undoStack et redoStack sont vides après un resetGame
        EXPECT_TRUE(jsonGame["undoStack"].empty());
        EXPECT_TRUE(jsonGame["redoStack"].empty());
    }*/


} // namespace hive::models

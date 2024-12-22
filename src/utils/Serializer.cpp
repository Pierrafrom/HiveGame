#include "nlohmann/json.hpp"
#include "models/Game.h"
#include <fstream>
#include "utils/Serializer.h"
#include "models/Move.h"
#include "models/PieceFactory.h"
#include <iomanip>
#include <sstream>
#include <filesystem>

namespace hive::utils {


    bool Serializer::saveGame(const hive::models::Game &game, const std::string &filename){

        nlohmann::json jsonGame;

        // Serialize players
        jsonGame["players"] = nlohmann::json::array();
        for (int i = 0; i < 2; i++) {
            const auto &player = game.getPlayer(i);
            jsonGame["players"].push_back({
                {"id", player.getId()},
                {"name", player.getName()}
            });
        }

        // Lambda to serialize a Move object
        auto serializeMove = [](const hive::models::Move &move) -> nlohmann::json {
            return {
                {"type", static_cast<int>(move.getType())}, //Finalement cette ligne est peut-être inutile
                {"player_id", move.getPlayer()->getId()},
                {"piece_type", static_cast<int>(move.getPiece()->getType())}, // Ajout du type de la pièce
                /*Le compilateur peut implicitement convertir une énumération en entier, car les énumérations
                 * sont basées sur des types entiers en C++.*/
                {"piece_id", move.getPiece()->getId()},
                {"from", move.getFrom() ? nlohmann::json{
                    {"x", move.getFrom()->getX()},
                    {"y", move.getFrom()->getY()},
                    {"z", move.getFrom()->getZ()}
                } : nullptr},
                {"to", {
                    {"x", move.getTo().getX()},
                    {"y", move.getTo().getY()},
                    {"z", move.getTo().getZ()}
                }}
            };
        };

        // Serialize undoStack
        jsonGame["undoStack"] = nlohmann::json::array();
        auto undoCopy = game.getUndoStack();
        while (!undoCopy.empty()) {
            jsonGame["undoStack"].push_back(serializeMove(*undoCopy.top()));
            undoCopy.pop();
        }

        // Serialize redoStack
        jsonGame["redoStack"] = nlohmann::json::array();
        auto redoCopy = game.getRedoStack();
        while (!redoCopy.empty()) {
            jsonGame["redoStack"].push_back(serializeMove(*redoCopy.top()));
            redoCopy.pop();
        }

        // Generate a default filename if none is provided
        std::string savePath;
        if (filename.empty()) {
            // Get the current timestamp
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);

            // Format the timestamp as YYYY-MM-DD_HH-MM
            std::stringstream ss;
            ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M");
            std::string timestamp = ss.str();

            // Construct the full path
            savePath = "data/game_save_" + timestamp + ".json";
        } else {
            savePath = filename;
        }

        // Ensure the "data" directory exists
        std::filesystem::create_directories("data");

        // Write to file
        std::ofstream file(savePath);
        if (!file.is_open()) {
            return false;
        }
        file << jsonGame.dump(4);
        file.close();

        return true;

    }

    bool Serializer::loadGame(hive::models::Game &game, const std::string &filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return false;
        }

        nlohmann::json jsonGame;
        file >> jsonGame;
        file.close();

        try {
            // 1. Recréer les joueurs
            auto player1 = std::make_shared<hive::models::Player>(
                    jsonGame["players"][0]["id"].get<size_t>(),
                    jsonGame["players"][0]["name"].get<std::string>()
            );

            auto player2 = std::make_shared<hive::models::Player>(
                    jsonGame["players"][1]["id"].get<size_t>(),
                    jsonGame["players"][1]["name"].get<std::string>()
            );

            // 2. Réinitialiser le jeu avec les nouveaux joueurs
            game.resetGame(player1->getName(), player2->getName());

            // Synchroniser les pointeurs des joueurs avec ceux du singleton Game
            auto player1_game = game.getPlayerPtr(0); // Récupérer les joueurs du singleton Game
            auto player2_game = game.getPlayerPtr(1);

            player1 = player1_game;
            player2 = player2_game;

            // 3. Recréer les pièces et les associer aux joueurs
            std::unordered_map<size_t, std::shared_ptr<hive::models::Piece>> pieceMap;

            auto deserializePiece = [&](const nlohmann::json &jsonMove) {
                size_t pieceId = jsonMove["piece_id"].get<size_t>();
                auto pieceType = static_cast<hive::models::enums::PieceType>(jsonMove["piece_type"].get<int>());

                // Créer la pièce avec la factory et convertir unique_ptr -> shared_ptr
                auto piece = std::shared_ptr<hive::models::Piece>(
                        hive::models::PieceFactory::createPiece(pieceType, pieceId).release()
                );

                if (jsonMove["player_id"] == player1->getId()) {
                    player1->addPiece(piece);
                } else if (jsonMove["player_id"] == player2->getId()) {
                    player2->addPiece(piece);
                } else {
                    throw std::runtime_error("Player ID not found during deserialization.");
                }

                // Stocker la pièce dans le map pour une réutilisation
                pieceMap[pieceId] = piece;
            };

            for (const auto &jsonMove : jsonGame["undoStack"]) {
                deserializePiece(jsonMove);
            }
            for (const auto &jsonMove : jsonGame["redoStack"]) {
                deserializePiece(jsonMove);
            }

            // 4. Recréer les mouvements
            auto deserializeMove = [&](const nlohmann::json &jsonMove) {
                auto player = (jsonMove["player_id"] == player1->getId()) ? player1 : player2;
                auto pieceId = jsonMove["piece_id"].get<size_t>();
                auto piece = pieceMap[pieceId];

                std::optional<hive::models::Hex> from = std::nullopt;
                if (!jsonMove["from"].is_null()) {
                    from = hive::models::Hex(
                            jsonMove["from"]["x"],
                            jsonMove["from"]["y"],
                            jsonMove["from"]["z"]
                    );
                }

                hive::models::Hex to(
                        jsonMove["to"]["x"],
                        jsonMove["to"]["y"],
                        jsonMove["to"]["z"]
                );

                if (from.has_value()) {
                    return std::make_shared<hive::models::Move>(player, piece, from.value(), to);
                } else {
                    return std::make_shared<hive::models::Move>(player, piece, to);
                }
            };

            // 5. Recréer undoStack
            auto &undoStack = game.getUndoStack();
            while (!undoStack.empty()) undoStack.pop();
            for (const auto &jsonMove : jsonGame["undoStack"]) {
                undoStack.push(deserializeMove(jsonMove));
            }

            // 6. Recréer redoStack
            auto &redoStack = game.getRedoStack();
            while (!redoStack.empty()) redoStack.pop();
            for (const auto &jsonMove : jsonGame["redoStack"]) {
                redoStack.push(deserializeMove(jsonMove));
            }

            // 7. Rejouer les mouvements du undoStack pour reconstruire l'état du plateau
            auto undoStackCopy = game.getUndoStack();
            std::vector<std::shared_ptr<hive::models::Move>> movesToReplay;

            while (!undoStackCopy.empty()) {
                movesToReplay.push_back(undoStackCopy.top());
                undoStackCopy.pop();
            }


            for (const auto &move : movesToReplay) {
                try {
                    move->execute(game.getBoard());
                    game.nextTurn();
                } catch (const std::exception &e) {
                    return false;
                }
            }

            std::cout << "Game loaded successfully!" << std::endl;
            return true;

        } catch (const std::exception &e) {
            std::cerr << "Error loading game: " << e.what() << std::endl;
            return false;
        }
    }

}



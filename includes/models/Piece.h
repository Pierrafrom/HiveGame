#ifndef PIECE_H
#define PIECE_H

#include <cstddef> // For size_t
#include <string>
#include <utility>
#include "models/enums/PieceType.h"
#include "Hex.h"
#include "Player.h"
#include "MoveStrategy.h"


namespace hive::models {
    /**
     * Reste à faire :
     * • reDéfinir la méthode move()
     * • reDéfinir la méthode getNeighbors()
     * • Changer le MoveStrategy des classes filles par <type>MoveStrategy
     *
     * @class Piece
     * @brief Represents a game piece in the Hive game.
     *
     * Each piece has a unique ID and possibly additional attributes
     * depending on the specific type of piece (e.g., Queen Bee, Beetle).
     *
     * /!\ Piece possède des classes filles dont les MoveSTrategy seront à remplacer lorsque l'on aura codé movestrat pour chacune d'entre elle
     *
     */
    class Piece {
    protected:
        size_t id; /**< Unique identifier for the piece */
        //std::string type; /**< Type of the piece (e.g., "Queen", "Beetle") */
        Hex position;
        const Player* player;
        MoveStrategy* moveStrategy;
        bool isImmobilized;

    public:
        /**
         * @brief Constructs a Piece with a given ID and type.
         * @param id The unique identifier for the piece.
         * @param type The type of the piece.
         */
        Piece(const size_t id, const Player* player, const Hex& posInitiale) : id(id), player(player), isImmobilized(false), moveStrategy(nullptr), position(posInitiale) {}
        ~Piece();
        Piece(const Piece& p) = delete;
        Piece& operator=(const Piece& p) = delete;

        //void move(); // --> verifier que liste des move strategy n'est pas vide


        //Méthode générales de la classe pièce
        void setMoveStrategy(MoveStrategy* strategy);
        [[nodiscard]] const MoveStrategy* getMoveStrategy() const {return moveStrategy;}

        /*void setPosition(const Hex& newPosition);*/
        [[nodiscard]] const Hex& getPosition() const {return position;}

        [[nodiscard]] const Player* getPlayer() const {return player;}

        [[nodiscard]] const bool getIsImmobilized() const {return isImmobilized;}
        void immobilize();
        void release();

        //std::vector<Piece*> getNeighbors();

        /**
         * @brief Retrieves the ID of the piece.
         * @return The ID of the piece.
         */
        [[nodiscard]] size_t getId() const { return id; }

        /**
         * @brief Retrieves the type of the piece.
         * @return The type as a string.
         */
        //[[nodiscard]] std::string getType() const { return type; }

        /**
         * @brief Equality operator for Piece.
         * @param other The other Piece to compare with.
         * @return True if both pieces have the same ID and type; otherwise, false.
         */
        bool operator==(const Piece &other) const {
            return id == other.id /*&& type == other.type*/;
        }

        /**
         * @brief Inequality operator for Piece.
         * @param other The other Piece to compare with.
         * @return True if pieces are not equal; otherwise, false.
         */
        bool operator!=(const Piece &other) const {
            return !(*this == other);
        }
    };
} // namespace hive::models


#endif // PIECE_H

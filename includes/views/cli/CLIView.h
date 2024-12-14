#ifndef HIVE_VIEWS_CLI_CLIVIEW_H
#define HIVE_VIEWS_CLI_CLIVIEW_H

#include <string>
#include <vector>
#include "models/Game.h"

namespace hive::views::cli {
    class CLIView {
    public:
        CLIView() = default;

        ~CLIView() = default;

        /**
         * @brief Affiche un message à l'utilisateur.
         */
        void displayMessage(const std::string &message);

        /**
         * @brief Affiche un message d'erreur.
         */
        void displayError(const std::string &error);

        /**
         * @brief Demande le nom d'un joueur.
         */
        [[nodiscard]] std::string askPlayerName(int playerNumber);

        /**
         * @brief Demande une commande à l'utilisateur.
         */
        [[nodiscard]] std::string getUserCommand();

        /**
         * @brief Demande un entier (par exemple pour un choix dans un menu).
         */
        [[nodiscard]] int askInt(const std::string &prompt) const;

        /**
         * @brief Affiche l'état complet du jeu avec une mise en page soignée.
         */
        void displayGameState(const hive::models::Game &game) const;

        /**
         * @brief Affiche une liste d'options et demande à l'utilisateur d'en sélectionner une.
         * @param prompt Message d'invite.
         * @param options Liste d'options.
         * @return L'index de l'option choisie.
         */
        [[nodiscard]] int askOption(const std::string &prompt, const std::vector<std::string> &options) const;

    private:
        // Méthodes utilitaires internes
        void printTitle(const std::string &title) const;

        static void printSeparator(char sep = '-', int length = 50) ;

        void printColored(const std::string &text, const std::string &color) const;

        static void displayBoard(const hive::models::Game &game) ;
    };
}

#endif // HIVE_VIEWS_CLI_CLIVIEW_H

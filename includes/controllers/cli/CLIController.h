#ifndef HIVELIB_CONTROLLERS_CLI_CLICONTROLLER_H
#define HIVELIB_CONTROLLERS_CLI_CLICONTROLLER_H

#include <memory>
#include "models/Game.h"
#include "views/cli/CLIView.h"

namespace hive::controllers::cli {
    class CLIController {
    public:
        explicit CLIController(std::shared_ptr<views::cli::CLIView> view);

        void run() const;

        bool loadGameFromFile(const std::string &filePath) const;

        void run(const std::string &saveFilePath) const;

    private:
        std::shared_ptr<views::cli::CLIView> view;
        models::Game &game;

        [[nodiscard]] bool handleCommand(const std::string &command) const;

        void placePiece() const;

        void movePiece() const;

        void undoMove() const;

        void redoMove() const;

        void displayGameState() const { view->displayGameState(game); }

        [[nodiscard]] models::enums::PieceType choosePieceType() const;

        [[nodiscard]] models::Hex chooseTargetHexFromMoves(const std::vector<models::Hex> &possibleMoves) const;
    };
}

#endif // HIVELIB_CONTROLLERS_CLI_CLICONTROLLER_H

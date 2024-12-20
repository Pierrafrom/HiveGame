#include "../src/controllers/gui/menu.h"
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <models/Game.h>
#include "models/Board.h"
#include "models/Player.h"
#include "../src/controllers/gui/playerinputdialog.h"
#include "../src/controllers/gui/gamewindow.h"
#include <models/PieceFactory.h>


Menu::Menu(QWidget *parent) : QWidget(parent)
{
    // Créer les widgets
    QLabel *title = new QLabel("Hive game");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold;");

    QPushButton *btnTwoPlayer = new QPushButton("2 Joueurs");
    QPushButton *btnSinglePlayer = new QPushButton("1 Joueur");
    QPushButton *btnImport = new QPushButton("Importer une partie");
    QPushButton *btnQuit = new QPushButton("Quitter");

    // Layout vertical
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(title);
    layout->addWidget(btnTwoPlayer);
    layout->addWidget(btnSinglePlayer);
    layout->addWidget(btnImport);
    layout->addWidget(btnQuit);

    setLayout(layout);

    // Connexion des signaux/slots
    connect(btnTwoPlayer, &QPushButton::clicked, this, &Menu::onStartTwoPlayerGame);
    connect(btnSinglePlayer, &QPushButton::clicked, this, &Menu::onStartSinglePlayerGame);
    connect(btnImport, &QPushButton::clicked, this, &Menu::onImportGame);
    connect(btnQuit, &QPushButton::clicked, this, &Menu::onQuitGame);
}

void Menu::onStartTwoPlayerGame() {
    PlayerInputDialog inputDialog(this);
    if (inputDialog.exec() == QDialog::Accepted) {
        QString player1Name = inputDialog.getPlayer1Name();
        QString player2Name = inputDialog.getPlayer2Name();

        // Récupérer l'instance de jeu et initialiser les joueurs
        auto &game = hive::models::Game::getInstance();
        game.resetGame(player1Name.toStdString(), player2Name.toStdString());

        // Récupérer les joueurs
        auto player1 = game.getPlayerPtr(0); // Joueur 1
        auto player2 = game.getPlayerPtr(1); // Joueur 2

        // Créer les pièces avec leurs propriétaires
        const hive::models::Hex origin(0, 0, 0);

        auto queen1 = hive::models::PieceFactory::createPiece(hive::models::enums::PieceType::QUEEN_BEE);
        auto ant1 = hive::models::PieceFactory::createPiece(hive::models::enums::PieceType::ANT);

        queen1->setOwner(player1); // Attribuer le joueur 1 à la pièce
        ant1->setOwner(player1); // Attribuer le joueur 1 à la pièce

        auto grasshoper1 = hive::models::PieceFactory::createPiece(hive::models::enums::PieceType::GRASSHOPPER);
        grasshoper1->setOwner(player2); // Attribuer le joueur 2 à la pièce

        // Ajouter les pièces au plateau
        const std::shared_ptr sharedQueen1 = std::move(queen1);
        const std::shared_ptr sharedgrasshoper1 = std::move(grasshoper1);
        const std::shared_ptr sharedAnt1 = std::move(ant1);

        gameBoard.addPiece(origin, sharedQueen1);
        gameBoard.addPiece(gameBoard.neighbor(origin, hive::models::enums::Direction::NORTH_EAST), sharedgrasshoper1);
        gameBoard.addPiece(gameBoard.neighbor(origin, hive::models::enums::Direction::WEST), sharedAnt1);

        // Créer et afficher la fenêtre de jeu
        GameWindow *gameWindow = new GameWindow(&game);
        gameWindow->setPlayerNames(player1Name, player2Name);
        gameWindow->show();

        // Fermer la fenêtre actuelle du menu
        this->close();
    }
}



void Menu::onStartSinglePlayerGame()
{
    emit startSinglePlayerGame(); // Signal pour démarrer une partie solo
}

void Menu::onImportGame()
{
    emit importGame(); // Signal pour importer une partie
}

void Menu::onQuitGame()
{
    emit quitGame(); //
}
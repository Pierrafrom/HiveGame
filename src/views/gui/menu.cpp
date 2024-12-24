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
#include "utils/Serializer.h"


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
        hive::models::Game &game = hive::models::Game::getInstance();
        game.resetGame(player1Name.toStdString(), player2Name.toStdString());

        // Récupérer les joueurs
        auto player1 = game.getPlayerPtr(0); // Joueur 1
        auto player2 = game.getPlayerPtr(1); // Joueur 2


        // Créer et afficher la fenêtre de jeu
        GameWindow *gameWindow = new GameWindow(&game);
        gameWindow->setPlayerNames(player1Name, player2Name);
        gameWindow->createDefautConfig();
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
    // Ouvre une boîte de dialogue pour sélectionner un fichier .json
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Importer une partie",
        "",
        "Fichiers JSON (*.json)"
    );

    if (!fileName.isEmpty()) {
        // Récupère l'instance unique de Game
        hive::models::Game &game = hive::models::Game::getInstance();

        // Récupère l'instance du Serializer
        hive::utils::Serializer &serialiser = hive::utils::Serializer::getInstance();

        // Appelle la fonction de chargement
        bool success = hive::utils::Serializer::loadGame(game, fileName.toStdString());
        qDebug() << fileName.toStdString();

        // Vérifie si le chargement a réussi
        if (success) {
            QMessageBox::information(
                this,
                "Import réussi",
                "La partie a été importée avec succès !"
            );

            // Crée la fenêtre de jeu avec la partie chargée
            GameWindow *gameWindow = new GameWindow(&game);

            gameWindow->show();

            // Ferme la fenêtre du menu si vous le souhaitez
            this->close();
        } else {
            QMessageBox::warning(
                this,
                "Échec de l'import",
                "Impossible de charger le fichier de sauvegarde."
            );
        }
    }
}


void Menu::onQuitGame()
{
    emit quitGame(); //
}
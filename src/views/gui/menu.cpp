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

Menu::Menu(QWidget *parent) : QWidget(parent)
{
    // Créer les widgets
    QLabel *title = new QLabel("Jeu de Plateau");
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

void Menu::onStartTwoPlayerGame()
{
    // Créer et afficher la boîte de dialogue
    PlayerInputDialog inputDialog(this);
    if (inputDialog.exec() == QDialog::Accepted) {
        QString player1Name = inputDialog.getPlayer1Name();
        QString player2Name = inputDialog.getPlayer2Name();

        // Accéder à l'instance Singleton et initialiser le jeu
        hive::models::Game &game = hive::models::Game::getInstance();
        game.initializeGame2players(player1Name.toStdString(), player2Name.toStdString());

        QMessageBox::information(this, "Jeu lancé",
            QString("Partie lancée avec %1 et %2").arg(QString::fromStdString(game.getPlayer(0).getName()), QString::fromStdString(game.getPlayer(1).getName())));
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
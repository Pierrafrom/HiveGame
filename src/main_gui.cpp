#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include "../src/controllers/gui/menu.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Menu menu;

    QObject::connect(&menu, &Menu::startTwoPlayerGame, []() {
        QMessageBox::information(nullptr, "2 Joueurs", "Lancement d'une partie pour 2 joueurs !");
        // Démarrez la logique de jeu pour 2 joueurs ici
    });

    QObject::connect(&menu, &Menu::startSinglePlayerGame, []() {
        QMessageBox::information(nullptr, "1 Joueur", "Lancement d'une partie pour 1 joueur !");
        // Démarrez la logique de jeu pour 1 joueur ici
    });

    QObject::connect(&menu, &Menu::importGame, []() {
        QString fileName = QFileDialog::getOpenFileName(nullptr, "Importer une partie", "", "Fichiers de sauvegarde (*.json)");
        if (!fileName.isEmpty()) {
            QMessageBox::information(nullptr, "Importer une partie", "Partie importée : " + fileName);
            // Ajoutez la logique pour charger la partie depuis le fichier ici
        }
    });

    QObject::connect(&menu, &Menu::quitGame, &app, &QApplication::quit);

    menu.show();

    return app.exec();
}

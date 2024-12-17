#include "../src/controllers/gui/gamewindow.h"

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Jeu Hive - Partie en cours");

    // Exemple d'affichage basique
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout();

    QLabel *label = new QLabel("La partie a commencé !");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    resize(800, 600);
}

void GameWindow::setPlayerNames(const QString &player1, const QString &player2) {
    QLabel *playersLabel = new QLabel(QString("Joueur 1: %1\nJoueur 2: %2").arg(player1, player2), this);
    playersLabel->setAlignment(Qt::AlignCenter);

    // Ajouter l'affichage des noms des joueurs
    QVBoxLayout *layout = dynamic_cast<QVBoxLayout *>(centralWidget()->layout());
    if (layout) {
        layout->addWidget(playersLabel);
    }
}

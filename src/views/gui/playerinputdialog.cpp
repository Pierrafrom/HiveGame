#include "../src/controllers/gui/playerinputdialog.h"

PlayerInputDialog::PlayerInputDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Entrer les noms des joueurs");

    QLabel *label1 = new QLabel("Nom du Joueur 1:");
    player1Input = new QLineEdit(this);

    QLabel *label2 = new QLabel("Nom du Joueur 2:");
    player2Input = new QLineEdit(this);

    QPushButton *confirmButton = new QPushButton("Lancer la partie", this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label1);
    layout->addWidget(player1Input);
    layout->addWidget(label2);
    layout->addWidget(player2Input);
    layout->addWidget(confirmButton);
    setLayout(layout);

    connect(confirmButton, &QPushButton::clicked, this, &QDialog::accept);
}

QString PlayerInputDialog::getPlayer1Name() const {
    return player1Input->text();
}

QString PlayerInputDialog::getPlayer2Name() const {
    return player2Input->text();
}

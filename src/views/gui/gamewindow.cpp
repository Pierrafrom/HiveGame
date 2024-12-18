#include "../src/controllers/gui/gamewindow.h"
#include <QGraphicsPolygonItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include <cmath>
#include <models/Board.h>
#include "../src/controllers/gui/HexGraphicsItem.h"
// include piece
#include <models/Piece.h>

GameWindow::GameWindow(hive::models::Board *board, QWidget *parent)
    : QMainWindow(parent), board(board) {
    setupUI();
    displayBoard();
}

void GameWindow::setupUI() {
    // Configuration générale
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Bandeau supérieur
    QHBoxLayout *topLayout = new QHBoxLayout();

    // Label pour le nombre de tours
    turnLabel = new QLabel("Tour : 1", this);
    topLayout->addWidget(turnLabel);

    // Espacement
    topLayout->addStretch();

    // Boutons Undo, Redo et Save
    QPushButton *undoButton = new QPushButton("Undo");
    QPushButton *redoButton = new QPushButton("Redo");
    QPushButton *saveButton = new QPushButton("Save");

    connect(undoButton, &QPushButton::clicked, this, &GameWindow::onUndoClicked);
    connect(redoButton, &QPushButton::clicked, this, &GameWindow::onRedoClicked);
    connect(saveButton, &QPushButton::clicked, this, &GameWindow::onSaveClicked);

    // Ajouter les boutons au bandeau supérieur
    topLayout->addWidget(undoButton);
    topLayout->addWidget(redoButton);
    topLayout->addWidget(saveButton);

    // Zone principale
    QHBoxLayout *mainContentLayout = new QHBoxLayout();
    gameBoardView = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    gameBoardView->setScene(scene);
    gameBoardView->setRenderHint(QPainter::Antialiasing);
    mainContentLayout->addWidget(gameBoardView);

    // Menu latéral
    QWidget *sideMenu = new QWidget(this);
    sideMenuLayout = new QVBoxLayout(sideMenu);

    // Ajouter les noms des joueurs dans le menu latéral
    QLabel *player1Label = new QLabel("Joueur 1 : Alice", this);
    QLabel *player2Label = new QLabel("Joueur 2 : Bob", this);
    sideMenuLayout->addWidget(player1Label);
    sideMenuLayout->addWidget(player2Label);
    sideMenuLayout->addStretch();

    mainContentLayout->addWidget(sideMenu);

    mainLayout->addLayout(topLayout);        // Bandeau supérieur
    mainLayout->addLayout(mainContentLayout); // Zone principale
    setCentralWidget(centralWidget);
}

void GameWindow::displayBoard() {
    scene->clear(); // Nettoyer la scène

    const int hexSize = 50; // Taille des hexagones
    const double sqrt3 = sqrt(3);

    // Parcourir les hexagones existants dans la table de hachage
    for (const auto &[hex, stack] : board->getAllHexes()) {
        // Calculer la position des hexagones en quinconce
        int x = hex.getX() * (hexSize * 1.5);
        int y = (hex.getY() - hex.getZ()) * (hexSize * sqrt3 / 2);

        // Créer un hexagone personnalisé
        QPolygonF hexagon;
        for (int i = 0; i < 6; ++i) {
            double angle = M_PI / 3 * i;
            hexagon << QPointF(hexSize * cos(angle), hexSize * sin(angle));
        }

        HexGraphicsItem *hexItem = new HexGraphicsItem(hex);
        hexItem->setPolygon(hexagon);
        hexItem->setPos(x, y);
        hexItem->setBrush(stack.empty() ? Qt::lightGray : Qt::yellow); // Couleur en fonction de l'état
        hexItem->setPen(QPen(Qt::black));

        // Connecter le signal de clic au traitement
        connect(hexItem, &HexGraphicsItem::hexClicked, this, [=](const hive::models::Hex &clickedHex) {
            qDebug() << "Hexagone cliqué à (" << clickedHex.getX() << ", " << clickedHex.getY() << ", " << clickedHex.getZ() << ")";
        });

        scene->addItem(hexItem);
    }
}

void GameWindow::onUndoClicked() {
    qDebug() << "Undo clicked";
    turnLabel->setText("Tour : " + QString::number(1)); // Exemple basique
}

void GameWindow::onRedoClicked() {
    qDebug() << "Redo clicked";
}

void GameWindow::onSaveClicked() {
    qDebug() << "Save clicked";
}

void GameWindow::setPlayerNames(const QString &player1, const QString &player2) {
    // Remplissage dynamique des labels de joueurs
    QLabel *player1Label = new QLabel(QString("Joueur 1 : %1").arg(player1), this);
    QLabel *player2Label = new QLabel(QString("Joueur 2 : %1").arg(player2), this);
    sideMenuLayout->addWidget(player1Label);
    sideMenuLayout->addWidget(player2Label);
}
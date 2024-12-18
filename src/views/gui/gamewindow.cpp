#include "../src/controllers/gui/gamewindow.h"
#include <QGraphicsPolygonItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include <cmath>
#include <models/Board.h>

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
    playerInfoLabel = new QLabel("Joueur 1 vs Joueur 2", this);
    turnLabel = new QLabel("Tour : 1", this);
    topLayout->addWidget(playerInfoLabel);
    topLayout->addStretch();
    topLayout->addWidget(turnLabel);

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
    QPushButton *undoButton = new QPushButton("Undo");
    QPushButton *redoButton = new QPushButton("Redo");
    QPushButton *saveButton = new QPushButton("Save");
    connect(undoButton, &QPushButton::clicked, this, &GameWindow::onUndoClicked);
    connect(redoButton, &QPushButton::clicked, this, &GameWindow::onRedoClicked);
    connect(saveButton, &QPushButton::clicked, this, &GameWindow::onSaveClicked);
    sideMenuLayout->addWidget(undoButton);
    sideMenuLayout->addWidget(redoButton);
    sideMenuLayout->addWidget(saveButton);
    sideMenuLayout->addStretch();
    mainContentLayout->addWidget(sideMenu);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(mainContentLayout);
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

        // Créer un hexagone
        QPolygonF hexagon;
        for (int i = 0; i < 6; ++i) {
            double angle = M_PI / 3 * i;
            hexagon << QPointF(hexSize * cos(angle), hexSize * sin(angle));
        }

        QGraphicsPolygonItem *hexItem = scene->addPolygon(hexagon, QPen(Qt::black), QBrush(Qt::lightGray));
        hexItem->setPos(x, y);

        // Interaction pour afficher les détails au clic
        hexItem->setFlag(QGraphicsItem::ItemIsSelectable);
        QObject::connect(scene, &QGraphicsScene::selectionChanged, [=]() {
            qDebug() << "Hex sélectionné à (" << hex.getX() << ", " << hex.getY() << ", " << hex.getZ() << ")";
        });

        // Si le hex contient une pièce, colorier l'hexagone
        if (!stack.empty()) {
            hexItem->setBrush(QBrush(Qt::yellow));
        }
    }
}

void GameWindow::onUndoClicked() {
    qDebug() << "Undo clicked";
}

void GameWindow::onRedoClicked() {
    qDebug() << "Redo clicked";
}

void GameWindow::onSaveClicked() {
    qDebug() << "Save clicked";
}

void GameWindow::setPlayerNames(const QString &player1, const QString &player2) {
    playerInfoLabel->setText(QString("Joueur 1 : %1 vs Joueur 2 : %2").arg(player1, player2));
}

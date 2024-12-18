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
#define hexSize 50

GameWindow::GameWindow(hive::models::Board *board, hive::models::Game *game, QWidget *parent)
    : QMainWindow(parent), board(board), game(game) {
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

    // Ajouter les noms des joueurs dans le menu latéral (dynamique)
    player1Label = new QLabel(this);
    player2Label = new QLabel(this);
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

        if (!stack.empty()) {
            auto piece = stack.top();
            hexItem->setBrush(piece->getOwner().getId() == 1 ? Qt::blue : Qt::red);
        }


        // Connecter le signal de clic au traitement
        connect(hexItem, &HexGraphicsItem::hexClicked, this, [=](const hive::models::Hex &clickedHex) {
         // Récupérer la pièce sur l'hexagone cliqué
         auto clickedPiece = board->getTopPiece(clickedHex);

         // Cas 1 : Désélectionner la pièce si elle est déjà sélectionnée
         if (selectedPiece && selectedPiece == clickedPiece) {
             qDebug() << "Deselecting piece at (" << clickedHex.getX() << ", " << clickedHex.getY() << ", " << clickedHex.getZ() << ")";
             selectedPiece.reset();
             clearPossibleMoves(); // Effacer les déplacements affichés
             return;
         }

         // Cas 2 : Sélectionner une nouvelle pièce
         if (clickedPiece) {
             selectedPiece = clickedPiece;
             qDebug() << "Selected piece at (" << clickedHex.getX() << ", " << clickedHex.getY() << ", " << clickedHex.getZ() << ")";
             displayPossibleMoves(*selectedPiece); // Afficher les déplacements valides
             return;
         }

         // Cas 3 : Déplacer la pièce si une destination valide est cliquée
         if (selectedPiece) {
             movePiece(clickedHex);
             nextTurn();
         }
     });



        scene->addItem(hexItem);
    }
}

void GameWindow::displayPossibleMoves(const hive::models::Piece &piece) {
    clearPossibleMoves(); // Nettoyer les surbrillances précédentes

    // Récupérer les déplacements valides
    validMoves = piece.getMoveStrategy().getPossibleMoves(*board, game->getCurrentPlayer());

    // Afficher les déplacements valides
    for (const auto &hex : validMoves) {
        int x = hex.getX() * (hexSize * 1.5);
        int y = (hex.getY() - hex.getZ()) * (hexSize * sqrt(3) / 2);

        // Ajouter une ellipse pour surligner l'hexagone
        auto highlight = scene->addEllipse(-hexSize / 2, -hexSize / 2, hexSize, hexSize,
                                           QPen(Qt::NoPen), QBrush(Qt::green, Qt::Dense4Pattern));
        highlight->setPos(x, y);

        moveHighlights.push_back(highlight);
    }
}



void GameWindow::clearPossibleMoves() {
    for (auto *highlight : moveHighlights) {
        scene->removeItem(highlight);
        delete highlight;
    }
    moveHighlights.clear();
}


void GameWindow::movePiece(const hive::models::Hex &to) {
    if (!selectedPiece) {
        qDebug() << "No piece selected.";
        return;
    }

    if (std::find(validMoves.begin(), validMoves.end(), to) == validMoves.end()) {
        qDebug() << "Invalid move.";
        return;
    }

    const auto from = selectedPiece->getPosition();
    if (!from) {
        qDebug() << "Selected piece has no position.";
        return;
    }

    try {
        board->movePiece(*from, to);         // Déplacer la pièce
        qDebug() << "Moved piece from (" << from->getX() << ", " << from->getY() << ", " << from->getZ() << ")"
                 << " to (" << to.getX() << ", " << to.getY() << ", " << to.getZ() << ")";
    } catch (const std::exception &e) {
        qDebug() << "Error moving piece: " << e.what();
    }

    selectedPiece.reset();
    validMoves.clear();
    clearPossibleMoves();
    displayBoard();
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
    if (game) {
        player1Label->setText("Joueur 1 : " + QString::fromStdString(game->getPlayer(0).getName()));
        player2Label->setText("Joueur 2 : " + QString::fromStdString(game->getPlayer(1).getName()));
    }
}

void GameWindow::getTurn() {
    int currentTurn = game->getTurnNumber();
    turnLabel->setText("Tour : " + QString::number(currentTurn));
}

void GameWindow::nextTurn() {
    game->nextTurn();
    getTurn();
}
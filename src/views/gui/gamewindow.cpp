#include "../src/controllers/gui/gamewindow.h"
#include <QGraphicsPolygonItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include <cmath>
#include <models/Board.h>
#include "../src/controllers/gui/HexGraphicsItem.h"
// include piece
#include <models/GameRules.h>
#include <models/Piece.h>
#define hexSize 50

GameWindow::GameWindow(hive::models::Game *game, QWidget *parent)
    : QMainWindow(parent), game(game) {
    setupUI();
    // displayBoard(); comment to don't create board twice
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

    // Boutons Undo, Redo et Save avec 'this' comme parent
    undoButton = new QPushButton("Undo", this);
    redoButton = new QPushButton("Redo", this);
    QPushButton *saveButton = new QPushButton("Save", this);

    // Connecter les signaux aux slots
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

    mainLayout->addLayout(topLayout); // Bandeau supérieur
    mainLayout->addLayout(mainContentLayout); // Zone principale
    setCentralWidget(centralWidget);

    // Initialisation du tour
    currentTurn = 1;
    updateUndoRedoButtons();
}

void GameWindow::displayPossibleMoves(const hive::models::Piece &piece) {
    if (!piece.getPosition()) {
        qDebug() << "Piece has no position.";
        return;
    }
    for (const auto &hex: validMoves) {
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
    for (auto *highlight: moveHighlights) {
        scene->removeItem(highlight);
        delete highlight;
    }
    moveHighlights.clear();
    validMoves.clear();
}

void GameWindow::updateValidMoves() {
    clearPossibleMoves();
    if (selectedPiece) {
        if (!selectedPiece->getPosition()) {
            qDebug() << "Piece has no position.";
            return;
        }
        validMoves = selectedPiece->getMoveStrategy().getPossibleMoves(game->getBoard(), game->getCurrentPlayer());
        for (auto it = validMoves.begin(); it != validMoves.end(); ) {
            hive::models::Move testMove(game->getPlayerPtr(), selectedPiece, selectedPiece->getPosition().value(), *it);
            qDebug() << "Testing move: " << testMove.toString();
            try {
                hive::models::GameRules::validateMove(testMove, game->getBoard(), game->getTurnNumber());
                ++it;
            } catch (const std::exception &e) {
                // log de l'exception
                qDebug() << e.what();
                it = validMoves.erase(it);
            }
        }

        displayPossibleMoves(*selectedPiece);
    }
}

void GameWindow::unselectPiece() {
    selectedPiece.reset();
    updateValidMoves();
}

void GameWindow::selectPiece(const hive::models::Hex &hex) {
    auto piece = game->getBoard().getTopPiece(hex);
    if (piece != nullptr) {
        if (piece->getOwner().getId() == game->getCurrentPlayer().getId()) {
            if (selectedPiece == piece) {
                qDebug() << "Deselecting";
                selectedPiece.reset();
                updateValidMoves();
            } else {
                clearPossibleMoves();
                selectedPiece = piece;
                updateValidMoves();
            }
        }
    }
}

void GameWindow::movePiece(const hive::models::Hex &to) {
    qDebug() << "debut de deplacement";
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

    // Exécuter le mouvement
    const hive::models::Move move(game->getPlayerPtr(), selectedPiece, from.value(), to);
    game->executeMove(move);

    unselectPiece();
    //displayBoard();
    getTurn();
    updateUndoRedoButtons();
    qDebug() << "fin de deplacement";
}

void GameWindow::displayBoard() {
    qDebug() << "display board";
    scene->clear(); // Nettoyer la scène
    const double sqrt3 = sqrt(3);

    // Parcourir les hexagones existants dans la table de hachage
    for (const auto &[hex, stack]: game->getBoard().getAllHexes()) {
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

            // Ajouter du texte pour la pièce
            QGraphicsTextItem *textItem = new QGraphicsTextItem(QString::fromStdString(toString(piece->getType())), hexItem);
            QFont font;
            font.setBold(true);
            font.setPointSize(10); // Taille de la police
            textItem->setFont(font);
            textItem->setDefaultTextColor(Qt::white); // Couleur du texte pour la lisibilité
            QRectF textRect = textItem->boundingRect();
            textItem->setPos(-textRect.width() / 2, -textRect.height() / 2);
        }

        // Connecter le signal de clic au traitement
        connect(hexItem, &HexGraphicsItem::hexClicked, this, [=](const hive::models::Hex clickedHex) {

            if (std::find(validMoves.begin(), validMoves.end(), clickedHex) != validMoves.end()) {
                // on déplace la pièce
                //updateValidMoves();
                movePiece(clickedHex);
            } else {
                selectPiece(clickedHex);
            }
        });
        qDebug() << "avant scene add item";
        scene->addItem(hexItem);
        qDebug() << "apres scene add item";
    }
        qDebug() << "t'as capté";
}

void GameWindow::onUndoClicked() {
    qDebug() << "Undo clicked";
    try {
        game->undo();
        displayBoard();
        getTurn();
        updateUndoRedoButtons();
    } catch (const std::exception &e) {
        // log de l'exception
        qDebug() << e.what();
    }
}

void GameWindow::updateUndoRedoButtons() {
    if (undoButton && redoButton && game) {
        undoButton->setEnabled(!game->getUndoStack().empty());
        redoButton->setEnabled(!game->getRedoStack().empty());
    }
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
    size_t currentTurn = game->getTurnNumber();
    turnLabel->setText("Tour : " + QString::number(currentTurn));
}

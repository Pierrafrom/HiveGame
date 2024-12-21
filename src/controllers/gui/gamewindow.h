#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <models/PieceFactory.h>

#include "models/Board.h"
#include "models/Game.h"

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GameWindow(hive::models::Game *game, QWidget *parent = nullptr);
    ~GameWindow() override = default;

    void setPlayerNames(const QString &player1, const QString &player2);

    void createDefautConfig() {
        const hive::models::Hex origin(0, 0, 0);
        auto queen1 = hive::models::PieceFactory::createPiece(hive::models::enums::PieceType::QUEEN_BEE);
        auto queen2 = hive::models::PieceFactory::createPiece(hive::models::enums::PieceType::QUEEN_BEE);

        queen1->setOwner(game->getPlayerPtr(0));
        queen2->setOwner(game->getPlayerPtr(1));

        const std::shared_ptr sharedQueen1 = std::move(queen1);
        const std::shared_ptr sharedQueen2 = std::move(queen2);

        game->getBoard().addPiece(origin, sharedQueen1);
        game->getBoard().addPiece(hive::models::Hex(1, -1, 0), sharedQueen2);

        displayBoard();
    }

    private slots:
    void onUndoClicked();
    void onRedoClicked();
    void onSaveClicked();
    void displayPossibleMoves(const hive::models::Piece &piece);
    void clearPossibleMoves(); // Nettoie les déplacements affichés

private:
    hive::models::Game *game; // Référence au jeu
    QPushButton *undoButton;
    QPushButton *redoButton;
    QLabel *playerInfoLabel;
    QLabel *turnLabel;
    QGraphicsView *gameBoardView;
    QGraphicsScene *scene;
    QVBoxLayout *sideMenuLayout;
    std::vector<QGraphicsEllipseItem *> moveHighlights; // Éléments graphiques pour les déplacements possibles
    QLabel *player1Label; // Label dynamique pour le joueur 1
    QLabel *player2Label; // Label dynamique pour le joueur 2
    int currentTurn;  // Tour actuel

    std::shared_ptr<hive::models::Piece> selectedPiece; // Pièce actuellement sélectionnée
    std::vector<hive::models::Hex> validMoves;


    void setupUI();           // Initialiser l'interface
    void selectPiece(const hive::models::Hex &hex);

    void updateValidMoves();

    void unselectPiece();

    void displayBoard();      // Afficher les hexagones existants
    void movePiece(const hive::models::Hex &to);
    void getTurn();
    void nextTurn();

    void updateUndoRedoButtons();
};


#endif // GAMEWINDOW_H

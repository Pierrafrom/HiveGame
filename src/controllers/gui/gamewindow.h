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

    // Petit helper pour créer une config par défaut
    void createDefautConfig() {
        const hive::models::Hex origin(0, 0, 0);
        auto queen1 = hive::models::PieceFactory::createPiece(hive::models::enums::PieceType::QUEEN_BEE);
        auto queen2 = hive::models::PieceFactory::createPiece(hive::models::enums::PieceType::QUEEN_BEE);

        queen1->setOwner(game->getPlayerPtr(0));
        queen2->setOwner(game->getPlayerPtr(1));

        const std::shared_ptr sharedQueen1 = std::move(queen1);
        const std::shared_ptr sharedQueen2 = std::move(queen2);

        game->getBoard().addPiece(hive::models::Hex(0, 0, 0), sharedQueen1);
        game->getBoard().addPiece(hive::models::Hex(1, -1, 0), sharedQueen2);

        displayBoard();
    }

private slots:
    void onUndoClicked();
    void onRedoClicked();
    void onSaveClicked();
    void displayPossibleMoves(const hive::models::Piece &piece);
    void clearPossibleMoves();

private:
    hive::models::Game *game;
    QPushButton *undoButton;
    QPushButton *redoButton;
    QLabel *playerInfoLabel;
    QLabel *turnLabel;
    QGraphicsView *gameBoardView;
    QGraphicsScene *scene;
    QVBoxLayout *sideMenuLayout;

    // Pour l'affichage du plateau
    std::vector<QGraphicsEllipseItem *> moveHighlights;
    QLabel *player1Label;
    QLabel *player2Label;
    int currentTurn;

    // Gestion des déplacements
    std::shared_ptr<hive::models::Piece> selectedPiece;
    std::vector<hive::models::Hex> validMoves;

    // Méthodes internes
    void setupUI();
    void selectPiece(const hive::models::Hex &hex);
    void updateValidMoves();
    void unselectPiece();
    void displayBoard();
    void movePiece(const hive::models::Hex &to);
    void getTurn();
    void nextTurn();
    void updateUndoRedoButtons();
};

#endif // GAMEWINDOW_H
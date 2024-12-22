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
#include <models/Move.h>
#include <models/GameRules.h>
#include <models/Hex.h>
#include <QGroupBox>

#include "models/Board.h"
#include "models/Game.h"

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GameWindow(hive::models::Game *game, QWidget *parent = nullptr);
    ~GameWindow() override = default;

    void setPlayerNames(const QString &player1, const QString &player2);
    void createDefautConfig();

    private slots:
        void onUndoClicked();
    void onRedoClicked();
    void onSaveClicked();
    void displayPossibleMoves(const hive::models::Piece &piece);
    void clearPossibleMoves();

    // Nouveau slot
    void onCreatePieceButtonClicked(int playerIndex, hive::models::enums::PieceType type);

private:
    hive::models::Game *game;
    QPushButton *undoButton;
    QPushButton *redoButton;
    QLabel *playerInfoLabel;
    QLabel *turnLabel;
    QGraphicsView *gameBoardView;
    QGraphicsScene *scene;
    QVBoxLayout *sideMenuLayout;

    std::vector<QGraphicsEllipseItem *> moveHighlights;
    QLabel *player1Label;
    QLabel *player2Label;
    int currentTurn;

    std::shared_ptr<hive::models::Piece> selectedPiece;
    std::vector<hive::models::Hex> validMoves;

    void setupUI();
    void selectPiece(const hive::models::Hex &hex);

    void selectPiece(const hive::models::Piece *piece);

    void updateValidMoves();
    void unselectPiece();
    void displayBoard();

    bool isSelectedPieceOnBoard() const;

    void movePiece(const hive::models::Hex &to);
    void getTurn();
    void nextTurn();
    void updateUndoRedoButtons();

    // Ajout
    QGroupBox* createPieceCreationGroupBox(int playerIndex);
    void updatePieceCreationButtons();
};


#endif // GAMEWINDOW_H
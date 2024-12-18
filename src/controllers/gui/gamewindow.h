#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include "models/Board.h"
#include "models/Game.h"

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GameWindow(hive::models::Board *board, hive::models::Game *game, QWidget *parent = nullptr);
    ~GameWindow() override = default;

    void setPlayerNames(const QString &player1, const QString &player2);

    private slots:
    void onUndoClicked();
    void onRedoClicked();
    void onSaveClicked();
    void displayPossibleMoves(const hive::models::Piece &piece);
    void clearPossibleMoves(); // Nettoie les déplacements affichés

private:
    hive::models::Game *game; // Référence au jeu
    QLabel *playerInfoLabel;
    QLabel *turnLabel;
    QGraphicsView *gameBoardView;
    QGraphicsScene *scene;
    QVBoxLayout *sideMenuLayout;
    hive::models::Board *board; // Référence au Board
    std::vector<QGraphicsEllipseItem *> moveHighlights; // Éléments graphiques pour les déplacements possibles
    QLabel *player1Label; // Label dynamique pour le joueur 1
    QLabel *player2Label; // Label dynamique pour le joueur 2
    int currentTurn;  // Tour actuel

    std::shared_ptr<hive::models::Piece> selectedPiece; // Pièce actuellement sélectionnée
    std::vector<hive::models::Hex> validMoves;


    void setupUI();           // Initialiser l'interface
    void displayBoard();      // Afficher les hexagones existants
    void movePiece(const hive::models::Hex &to);
    void getTurn();
    void nextTurn();
};

#endif // GAMEWINDOW_H

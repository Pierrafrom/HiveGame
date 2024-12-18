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


    void setupUI();           // Initialiser l'interface
    void displayBoard();      // Afficher les hexagones existants
};

#endif // GAMEWINDOW_H

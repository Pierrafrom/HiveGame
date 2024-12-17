#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);

    void setPlayerNames(const QString &player1, const QString &player2);
};

#endif // GAMEWINDOW_H

#ifndef MENU_H
#define MENU_H

#include <QWidget>

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);

    signals:
    void startTwoPlayerGame();
    void startSinglePlayerGame();
    void importGame();
    void quitGame();

    public slots:
    void onStartTwoPlayerGame();
    void onStartSinglePlayerGame();
    void onImportGame();
    void onQuitGame();
};

#endif // MENU_H

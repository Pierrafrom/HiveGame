#ifndef PLAYERINPUTDIALOG_H
#define PLAYERINPUTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class PlayerInputDialog : public QDialog {
    Q_OBJECT

public:
    explicit PlayerInputDialog(QWidget *parent = nullptr);

    QString getPlayer1Name() const;
    QString getPlayer2Name() const;

private:
    QLineEdit *player1Input;
    QLineEdit *player2Input;
};

#endif // PLAYERINPUTDIALOG_H

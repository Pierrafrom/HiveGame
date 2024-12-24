#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QWidget>
class QGridLayout;

namespace hive::views {
    class BoardView : public QWidget {
        Q_OBJECT
        QGridLayout* layoutHex;
    public:
        explicit BoardView(QWidget *parent = nullptr);

    };
}


#endif // BOARDVIEW_H

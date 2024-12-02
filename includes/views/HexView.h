#ifndef HEXVIEW_H
#define HEXVIEW_H

#include <QWidget>

namespace hive::views {
    class HexView : public QWidget{
    private:
        Q_OBJECT
        int _x;
        int _y;
        int _z;
    public:
        explicit HexView(QWidget *parent = nullptr, int x = 0, int y = 0, int z = 0);
    protected:
        void paintEvent(QPaintEvent *event) override; // Paint event declaration
    };
}

#endif //HEXVIEW_H

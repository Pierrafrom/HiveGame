#include "views/HexView.h"
#include <QPainter>
#include <QPolygon>

namespace hive::views {
    HexView::HexView(QWidget *parent, int x, int y, int z) : QWidget(parent), _x(x), _y(y), _z(z) {
        setFixedSize(200, 200);
    }


    void HexView::paintEvent(QPaintEvent *event) {
        QWidget::paintEvent(event);

        QPainter painter(this);
        QPolygon hexagon;

        int centerX = width() / 2;
        int centerY = height() / 2;
        int radius = qMin(width(), height()) / 2 - 10; // Ensure it fits the widget

        for (int i = 0; i < 6; ++i) {
            double angle = M_PI / 3.0 * i;
            int x = static_cast<int>(centerX + radius * cos(angle));
            int y = static_cast<int>(centerY + radius * sin(angle));
            hexagon << QPoint(x, y);
        }

        painter.setBrush(QBrush(Qt::cyan));
        painter.setPen(QPen(Qt::black, 2));
        painter.drawPolygon(hexagon);
    }
}
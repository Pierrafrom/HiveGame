#include "views\BoardView.h"
#include <QGuiApplication>
#include <QScreen>
#include <QRect>
#include <QHBoxLayout>

#include "views/HexView.h"

namespace hive::views {
    BoardView::BoardView(QWidget *parent) : QWidget(parent) {
        // Get the screen geometry
        QScreen *screen = QGuiApplication::primaryScreen();
        if (screen) {
            QRect screenGeometry = screen->geometry();

            // Calculate 90% of the screen size
            int width = static_cast<int>(screenGeometry.width() * 0.8);
            int height = static_cast<int>(screenGeometry.height() * 0.8);

            // Calculate the position to center the window
            int x = (screenGeometry.width() - width) / 2;
            int y = (screenGeometry.height() - height) / 2;

            // Set the geometry (position and size) of the window
            setGeometry(x, y, width, height);
        }
        layoutHex = new QGridLayout;
        layoutHex->addWidget(new HexView, 0, 0);
        setLayout(layoutHex);
        layoutHex->addWidget(new HexView, 1, 1);
    }
}


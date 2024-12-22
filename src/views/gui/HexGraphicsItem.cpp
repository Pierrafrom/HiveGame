#include "../src/controllers/gui/HexGraphicsItem.h"
#include <QGraphicsSceneMouseEvent>


HexGraphicsItem::HexGraphicsItem(const hive::models::Hex &hex, QGraphicsItem *parent)
    : QObject(), QGraphicsPolygonItem(parent), hexCoord(hex) {}

void HexGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit hexClicked(hexCoord);
    QGraphicsPolygonItem::mousePressEvent(event);
}
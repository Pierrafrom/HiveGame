#include "../src/controllers/gui/HexGraphicsItem.h"
#include <QGraphicsSceneMouseEvent>

HexGraphicsItem::HexGraphicsItem(const hive::models::Hex &hex, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent), hex(hex) {}

void HexGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit hexClicked(hex); // Émet le signal avec les coordonnées du hexagone cliqué
    QGraphicsPolygonItem::mousePressEvent(event); // Appel de la méthode parente pour conserver le comportement par défaut
}

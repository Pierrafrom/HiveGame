#ifndef HEXGRAPHICSITEM_H
#define HEXGRAPHICSITEM_H

#include <QGraphicsPolygonItem>
#include <QObject>
#include "models/Hex.h"

class HexGraphicsItem : public QObject, public QGraphicsPolygonItem {
    Q_OBJECT
public:
    explicit HexGraphicsItem(const hive::models::Hex &hex, QGraphicsItem *parent = nullptr);

    signals:
        void hexClicked(const hive::models::Hex &hex);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    hive::models::Hex hexCoord;
};


#endif // HEXGRAPHICSITEM_H

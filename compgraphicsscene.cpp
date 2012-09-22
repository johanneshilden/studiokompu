#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "compgraphicsscene.h"

CompGraphicsScene::CompGraphicsScene(QObject *parent)
    : QGraphicsScene(parent),
      m_dropItem(0)
{
}

CompNodeItem *CompGraphicsScene::selectedNodeItem() const
{
    QList<QGraphicsItem *> items = selectedItems();
    if (items.isEmpty())
        return 0;
    return qgraphicsitem_cast<CompNodeItem *>(items.first());
}

void CompGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    CompNodeItem *item;
    item = qgraphicsitem_cast<CompNodeItem *>(itemAt(event->scenePos().x(),
                                                     event->scenePos().y()));
    QList<QGraphicsItem *> items = selectedItems();
    if (!items.isEmpty())
        items.first()->setSelected(false);
    if (item) {
        QPointF p = item->scenePos();
        const qreal s = item->sceneTransform().m22();
        QRectF box(p.x() - 25 * s, p.y() - 25 * s, 50 * s, 50 * s);
        if (box.contains(event->scenePos()))
            item->setSelected(true);
    }
}

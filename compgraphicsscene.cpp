#include <QGraphicsSceneMouseEvent>
#include "compgraphicsscene.h"

CompGraphicsScene::CompGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
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
     QGraphicsItem *item = itemAt(event->scenePos().x(), event->scenePos().y());
     QList<QGraphicsItem *> items = selectedItems();
     if (!items.isEmpty())
         items.first()->setSelected(false);
     if (item)
         item->setSelected(true);
}

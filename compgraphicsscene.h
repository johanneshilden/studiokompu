#ifndef COMPGRAPHICSSCENE_H
#define COMPGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "compnodeitem.h"

class CompGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit CompGraphicsScene(QObject *parent = 0);
    CompNodeItem *selectedNodeItem() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // COMPGRAPHICSSCENE_H

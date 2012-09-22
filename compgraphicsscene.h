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

    inline void setDropItem(CompNodeItem *item) { m_dropItem = item; }
    inline CompNodeItem *dropItem() const { return m_dropItem; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    CompNodeItem *m_dropItem;
};

#endif // COMPGRAPHICSSCENE_H

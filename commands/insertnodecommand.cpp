#include <QGraphicsScene>
#include <QDebug>
#include "insertnodecommand.h"
#include "compnodeitem.h"

InsertNodeCommand::InsertNodeCommand(CompWidget *widget, CompNodeItem *item, CompNodeItem *parentItem, int n, QUndoCommand *parent)
    : QUndoCommand(parent),
      m_widget(widget),
      m_item(item),
      m_parent(parentItem),
      m_tmp(0),
      m_comp((struct node_composition *) parentItem->node()->data),
      m_pos(n)
{
    item->setParentItem(parentItem);
    setText(QObject::tr("Insert node"));
}

InsertNodeCommand::~InsertNodeCommand()
{
    delete m_tmp;
}

void InsertNodeCommand::undo()
{
    struct node **g = m_comp->g;
    struct node **h = node_array_new(m_comp->places);

    int j;
    for (j = 0; j < m_comp->places; ++j) {
        if (m_pos != j)
            h[j] = *g;
        ++g;
    }
    h[j] = NULL;
    free(m_comp->g);
    m_comp->g = h;
    --m_comp->places;

    m_tmp = m_parent->m_childNodes.takeAt(m_pos + 1);
    m_tmp->scene()->removeItem(m_tmp);

    rebuild();
}

void InsertNodeCommand::redo()
{
    struct node **g = m_comp->g;
    struct node **h = node_array_new(m_comp->places + 2);

    int j;
    for (j = 0; j < m_comp->places + 1; ++j) {
        if (m_pos == j) {
            h[j] = m_item->node();
        } else {
            h[j] = *g;
            ++g;
        }
    }
    h[j] = NULL;
    free(m_comp->g);
    m_comp->g = h;
    ++m_comp->places;

    m_parent->m_childNodes.insert(m_pos + 1, m_item);
    m_item->setParentItem(m_parent);
    m_tmp = 0;

    rebuild();
}

void InsertNodeCommand::rebuild()
{
    QList<QGraphicsItem *> items = m_parent->childItems();
    QList<QGraphicsItem *>::const_iterator k;
    for (k = items.constBegin(); k != items.constEnd(); ++k) {
        QGraphicsItem *it = *k;

        if (!qgraphicsitem_cast<CompNodeItem *>(it)) {
            // Item is not a CompNodeItem
            it->scene()->removeItem(it);
            delete it;
        }
    }

    m_parent->buildCompositionNodeLegs();
}

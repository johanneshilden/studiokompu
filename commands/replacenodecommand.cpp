#include <QGraphicsScene>
#include <QDebug>
#include <cassert>
#include "replacenodecommand.h"
#include "compnodeitem.h"
#include "compwidget.h"

ReplaceNodeCommand::ReplaceNodeCommand(CompWidget *widget, CompNodeItem *old, CompNodeItem *item, QUndoCommand *parent)
    : QUndoCommand(parent),
      m_widget(widget),
      m_old(old),
      m_new(item),
      m_item(0)
{
    setText(QObject::tr("Replace node"));
}

ReplaceNodeCommand::~ReplaceNodeCommand()
{
    delete m_item;
}

void ReplaceNodeCommand::undo()
{
    m_item = switchNodes(m_new, m_old);
}

void ReplaceNodeCommand::redo()
{
    m_item = switchNodes(m_old, m_new);
}

CompNodeItem *ReplaceNodeCommand::switchNodes(CompNodeItem *old, CompNodeItem *neue)
{
    CompNodeItem *parentNode = qgraphicsitem_cast<CompNodeItem *>(old->parentItem());
    QGraphicsScene *scene = old->scene();

    neue->setScale(old->scale());
    neue->setPos(old->pos());
    neue->setParentItem(parentNode);
    scene->removeItem(old);

    if (parentNode) {
        const int n = parentNode->childNodes().indexOf(old);
        parentNode->m_childNodes.replace(n, neue);
        struct node *node = neue->node();

        switch (parentNode->nodeType())
        {
        case CompNodeItem::CompositionNode:
        {
            struct node_composition *comp = (struct node_composition *) parentNode->node()->data;
            assert(n <= comp->places);
            (n ? comp->g[n - 1] : comp->f) = node;
            break;
        }
        case CompNodeItem::RecursionNode:
        {
            assert(n < 2);
            struct node_recursion *rec = (struct node_recursion *) parentNode->node()->data;
            (n ? rec->g : rec->f) = node;
            break;
        }
        case CompNodeItem::SearchNode:
        {
            assert(0 == n);
            struct node_search *search = (struct node_search *) parentNode->node()->data;
            search->p = node;
            break;
        }
        default:
            qWarning() << "Invalid node type in ReplaceNodeCommand::switchNodes";
            break;
        }
    } else {
        scene->addItem(neue);
        m_widget->setTopNode(neue);
    }

    foreach (QGraphicsItem *item, scene->selectedItems())
        item->setSelected(false);
    neue->setSelected(true);
    scene->update();
    m_widget->validate();
    return old;
}

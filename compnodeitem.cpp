#include <QPainter>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>
#include <cassert>
#include "compnodeitem.h"

void node_destroy_no_cascade(struct node *n)
{
    struct node_composition *comp;

    printf("--NODE_DESTROY\n");
    fflush(stdout);

    if (!n)
        return;
    switch (n->type)
    {
    case NODE_COMPOSITION:
        comp = (struct node_composition *) n->data;
        free(comp->g);
    case NODE_RECURSION:
    case NODE_SEARCH:
    case NODE_ZERO:
    case NODE_PROJECTION:
    case NODE_SUCCESSOR:
    case NODE_INVALID:
        break;
    }
    free(n->data);
    free(n);
}

qreal const CompNodeItem::NodeScaleFactor = 0.7;

CompNodeItem::CompNodeItem(struct node *node, QGraphicsItem *parent)
    : QGraphicsItem(parent),
      m_node(node)
{
    switch (nodeType())
    {
    case CompositionNode:
    {
        struct node_composition *comp = (struct node_composition *) node->data;
        m_childNodes.push_back(new CompNodeItem(comp->f, this));

        struct node **g = comp->g;
        while (*g) {
            m_childNodes.push_back(new CompNodeItem(*g, this));
            ++g;
        }

        buildCompositionNodeLegs();
        break;
    }
    case RecursionNode:
    {
        struct node_recursion *rec = (struct node_recursion *) node->data;
        m_childNodes.push_back(new CompNodeItem(rec->f, this));
        m_childNodes.push_back(new CompNodeItem(rec->g, this));

        buildRecursionNodeLegs();
        break;
    }
    case SearchNode:
    {
        struct node_search *search = (struct node_search *) node->data;
        m_childNodes.push_back(new CompNodeItem(search->p, this));

        buildSearchNodeLeg();
        break;
    }
    case ZeroNode:
    case ProjectionNode:
    case SuccessorNode:
    case InvalidNode:
    default:
        break;
    } // end switch

    setFlag(QGraphicsItem::ItemIsSelectable);
}

CompNodeItem::~CompNodeItem()
{
    node_destroy_no_cascade(m_node);
}

bool CompNodeItem::isLeaf() const
{
    const CompNodeItem::NodeType type = nodeType();
    return (SuccessorNode  == type
         || ProjectionNode == type
         || ZeroNode       == type
         || InvalidNode    == type);
}

bool CompNodeItem::isValid() const
{
    switch (nodeType())
    {
    case ZeroNode:
        return true;
    case ProjectionNode:
        return true;
    case SuccessorNode:
        return true;
    case CompositionNode:
    case RecursionNode:
    case SearchNode:
    {
        QList<CompNodeItem *>::const_iterator i;
        for (i = m_childNodes.constBegin(); i != m_childNodes.constEnd(); ++i)
            if (!(*i)->isValid())
                return false;
        return true;
    }
    case InvalidNode:
    default:
        return false;
    } // end switch
}

int CompNodeItem::compute() const
{
    return node_compute(m_node, NULL, 0);
}

int CompNodeItem::compute(QList<int> args) const
{
    const int n = args.size();
    if (!n)
        return compute();
    int x[n];
    for (int i = 0; i < n; ++i)
        x[i] = args.at(i);
    return node_compute(m_node, x, n);
}

QRectF CompNodeItem::boundingRect() const
{
    return QRectF(-25, -25, 50, 50);
}

void CompNodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen pen;
    if (isSelected())
        pen.setColor(Qt::cyan);

    painter->setPen(pen);

    switch (nodeType())
    {
    case ZeroNode:
        painter->drawRect(boundingRect());
        painter->drawText(0, 0, "Z");
        break;
    case ProjectionNode:
    {
        painter->drawRect(boundingRect());
        struct node_projection *proj = (struct node_projection *) node()->data;
        painter->drawText(0, 0, QString("P:%1").arg(proj->place + 1));
        break;
    }
    case SuccessorNode:
        painter->drawRect(boundingRect());
        painter->drawText(0, 0, "+");
        break;
    case CompositionNode:
        painter->drawRect(boundingRect());
        painter->drawText(0, 0, "C");
        break;
    case RecursionNode:
        painter->drawRect(boundingRect());
        painter->drawText(0, 0, "R");
        break;
    case SearchNode:
        painter->drawRect(boundingRect());
        painter->drawText(0, 0, "S");
        break;
    case InvalidNode:
    default:
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        painter->drawRect(boundingRect());
        break;
    } // end switch
}

int CompNodeItem::type() const
{
    return CompNodeItem::Type;
}

CompNodeItem::NodeType CompNodeItem::nodeType() const
{
    return static_cast<CompNodeItem::NodeType>(m_node->type);
}

void CompNodeItem::buildCompositionNodeLegs()
{
    assert(m_childNodes.size() > 1);

    const qreal sf = NodeScaleFactor * (qreal) 4/(m_childNodes.count() + 2);

    CompNodeItem *f = m_childNodes.first();
    f->setPos(-65, 95);
    f->setScale(sf);
    new QGraphicsLineItem(0, 25, f->x(), f->y() - 25 * sf, this);

    qreal d = (qreal) 130/(m_childNodes.count() - 1);
    qreal x = -65 + d;

    QList<CompNodeItem *>::const_iterator i;
    for (i = m_childNodes.constBegin() + 1; i != m_childNodes.constEnd(); ++i) {
        CompNodeItem *n = *i;
        n->setPos(x, 95);
        n->setScale(sf);
        new QGraphicsLineItem(0, 25, n->x(), n->y() - 25 * sf, this);
        x += d;
    }
}

void CompNodeItem::buildRecursionNodeLegs()
{
    assert(m_childNodes.size() == 2);

    CompNodeItem *f = m_childNodes.at(0);
    CompNodeItem *g = m_childNodes.at(1);

    f->setPos(-65, 95);
    g->setPos(65, 95);
    f->setScale(CompNodeItem::NodeScaleFactor);
    g->setScale(CompNodeItem::NodeScaleFactor);

    new QGraphicsLineItem(0, 25, f->x(), f->y() - 25 * NodeScaleFactor, this);
    new QGraphicsLineItem(0, 25, g->x(), g->y() - 25 * NodeScaleFactor, this);
}

void CompNodeItem::buildSearchNodeLeg()
{
    assert(m_childNodes.size() == 1);

    CompNodeItem *p = m_childNodes.first();

    p->setPos(0, 95);
    p->setScale(CompNodeItem::NodeScaleFactor);

    new QGraphicsLineItem(0, 25, 0, p->y() - 25 * NodeScaleFactor, this);
}

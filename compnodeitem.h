#ifndef COMPNODEITEM_H
#define COMPNODEITEM_H

#include <QGraphicsItem>
#include <QSharedPointer>
#include "lib/comp.h"
#include <tr1/memory>
#include <tr1/shared_ptr.h>

using namespace std;

void node_destroy_no_cascade(struct node *n);

//struct free_delete
//{
//    void operator ()(struct node *node) { node_destroy_no_cascade(node); }
//};

class CompNodeItem : public QGraphicsItem
{
public:
    static qreal const NodeScaleFactor;

    enum { Type = UserType + 1 };

    enum NodeType {
        ZeroNode        = NODE_ZERO,
        ProjectionNode  = NODE_PROJECTION,
        SuccessorNode   = NODE_SUCCESSOR,
        CompositionNode = NODE_COMPOSITION,
        RecursionNode   = NODE_RECURSION,
        SearchNode      = NODE_SEARCH,
        InvalidNode     = NODE_INVALID
    };

    explicit CompNodeItem(struct node *node, QGraphicsItem *parent = 0);
    ~CompNodeItem();

    bool isLeaf() const;

    int compute() const;
    int compute(QList<int> args) const;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    int type() const;

    inline struct node *node() const { return m_node; }
    CompNodeItem::NodeType nodeType() const;

    inline QList<CompNodeItem *> childNodes() const { return m_childNodes; }
    void insertChildNode(int i, struct node *node);

private:
    friend class ReplaceNodeCommand;

    void buildCompositionNodeLegs();
    void buildRecursionNodeLegs();
    void buildSearchNodeLeg();

    struct node *m_node;
    QList<CompNodeItem *> m_childNodes;
};

#endif // COMPNODEITEM_H

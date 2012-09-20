#include <QDebug>
#include "complibmodel.h"

CompLibNode::CompLibNode(QString title)
    : m_title(title),
      m_parent(0)
{
}

CompLibNode::~CompLibNode()
{
    while (!m_children.isEmpty())
        delete m_children.takeLast();
}

int CompLibNode::row() const
{
    if (m_parent)
        return m_parent->m_children.indexOf(const_cast<CompLibNode *>(this));
    return 0;
}

void CompLibNode::insert(int position, CompLibNode *item)
{
    if (position > m_children.count()) {
        qWarning() << "Child index out of range in CompLibNode::insert";
        return;
    }
    item->m_parent = this;
    m_children.insert(position, item);
}

CompLibModel::CompLibModel(QObject *parent)
    : QAbstractItemModel(parent),
      m_rootNode(new CompLibNode("root"))
{
    CompLibNode *item1 = new CompLibNode("item #1");
    m_rootNode->insert(m_rootNode->childCount(), item1);

    CompLibNode *item2 = new CompLibNode("item #2");
    m_rootNode->insert(m_rootNode->childCount(), item2);

    CompLibNode *item3 = new CompLibNode("item #3");
    m_rootNode->insert(m_rootNode->childCount(), item3);

    CompLibNode *item4 = new CompLibNode("item #4");
    m_rootNode->insert(m_rootNode->childCount(), item4);
}

CompLibModel::~CompLibModel()
{
    delete m_rootNode;
}

QModelIndex CompLibModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();
    CompLibNode *childItem = node(parent)->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex CompLibModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    CompLibNode *childItem = static_cast<CompLibNode *>(child.internalPointer());
    CompLibNode *parentItem = childItem->parent();

    if (parentItem == m_rootNode)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int CompLibModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;
    return node(parent)->childCount();
}

int CompLibModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant CompLibModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    CompLibNode *item = static_cast<CompLibNode *>(index.internalPointer());
    switch (role)
    {
    case Qt::DisplayRole:
        return item->title();
    case Qt::SizeHintRole:
    case Qt::DecorationRole:
//    case CompLibModel::ExpressionRole:
//    case CompLibModel::NodeTypeRole:
    default:
        break;
    }  // end switch
    return QVariant();
}

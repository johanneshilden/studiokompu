#include <QDebug>
#include "complibmodel.h"
#include "compnodeitem.h"

CompLibNode::CompLibNode()
    : m_parent(0)
//      m_nodeItem(nodeItem)
{
//    qDebug() << ">>>>>>>> ++ CompLibNode";
}

CompLibNode::~CompLibNode()
{
    while (!m_children.isEmpty())
        delete m_children.takeLast();
//    delete m_nodeItem;

//    qDebug() << ">>>>>>>> -- CompLibNode";
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

CompLibNode *CompLibNode::takeAt(int position)
{
    if (position > m_children.count()) {
        qWarning() << "Child index out of range in CompLibNode::takeAt";
        return 0;
    }
    CompLibNode *item = m_children.takeAt(position);
    item->m_parent = 0;
    return item;
}

CompLibModel::CompLibModel(QObject *parent)
    : QAbstractItemModel(parent),
      m_rootNode(new CompLibNode)
{
    //

//    insertRow(0, QModelIndex());
//    QModelIndex i = index(0, 0);
//    setData(i, "first row");

//    insertRow(0, QModelIndex());
//    i = index(0, 0);
//    setData(i, "another row");

//    insertRow(0, QModelIndex());
//    i = index(0, 0);
//    setData(i, "yet another row");

//    //

//    removeRow(0, QModelIndex());
//    removeRow(0, QModelIndex());
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

bool CompLibModel::hasChildren(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return true;
    CompLibNode *parentItem = static_cast<CompLibNode *>(parent.internalPointer());
//    if (!parentItem->isFolder())
//        return false;
    return (parentItem->childCount() > 0);
}

bool CompLibModel::insertRow(int row, const QModelIndex &parent)
{
    CompLibNode *item = node(parent);
    if (row > item->childCount()) {
        qWarning() << "Row index out of range in CompLibModel::insertRow()";
        return false;
    }

    beginInsertRows(parent, row, row);
    item->insert(row, new CompLibNode);
    endInsertRows();
    return true;
}

bool CompLibModel::removeRow(int row, const QModelIndex &parent)
{
    CompLibNode *item = node(parent);
    if (row >= item->childCount()) {
        qWarning() << "Row index out of range in CompLibModel::removeRow()";
        return false;
    }

    beginRemoveRows(parent, row, row);
    CompLibNode *node = item->takeAt(row);
    endRemoveRows();
    delete node;
    return true;
}

bool CompLibModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    CompLibNode *item = node(index);
    switch (role)
    {
    case Qt::EditRole:
//        m_undoStack->push(new LRenameCommand(this, item, value.toString()));
        item->setTitle(value.toString());
        emit dataChanged(index, index);
        return true;
    case CompLibModel::NodeDataRole:
        item->setSerialData(value.toString());
        emit dataChanged(index, index);
        return true;
    default:
        break;
    }  // end switch
    return false;
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
    case CompLibModel::NodeDataRole:
        return item->serialData();
    case Qt::SizeHintRole:
    case Qt::DecorationRole:
    default:
        break;
    }  // end switch
    return QVariant();
}

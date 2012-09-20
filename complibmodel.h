#ifndef COMPLIBMODEL_H
#define COMPLIBMODEL_H

#include <QAbstractItemModel>

class CompLibNode
{
public:
    CompLibNode(QString title);
    ~CompLibNode();

    inline void setTitle(QString title) { m_title = title; }
    inline QString title() const { return m_title; }

    inline CompLibNode *child(int row) const { return m_children.value(row); }
    inline int childCount() const { return m_children.count(); }

    inline CompLibNode *parent() const { return m_parent; }

    int row() const;
    void insert(int position, CompLibNode *item);

private:
    QString m_title;
    QList<CompLibNode *> m_children;
    CompLibNode *m_parent;
};


class CompLibModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit CompLibModel(QObject *parent = 0);
    ~CompLibModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    inline CompLibNode *node(const QModelIndex &index) const
    {
        if (index.isValid())
            return static_cast<CompLibNode *>(index.internalPointer());
        else
            return m_rootNode;
    }

private:
    CompLibNode *const m_rootNode;
};

#endif // COMPLIBMODEL_H

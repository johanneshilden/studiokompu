#ifndef COMPLIBMODEL_H
#define COMPLIBMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

class CompNodeItem;

class CompLibNode
{
public:
    CompLibNode();
    ~CompLibNode();

    inline void setTitle(QString title) { m_title = title; }
    inline QString title() const { return m_title; }

    inline void setSerialData(QString data) { m_serial = data; }
    inline QString serialData() const { return m_serial; }

    inline CompLibNode *child(int row) const { return m_children.value(row); }
    inline int childCount() const { return m_children.count(); }

    inline CompLibNode *parent() const { return m_parent; }

    int row() const;
    void insert(int position, CompLibNode *item);
    CompLibNode *takeAt(int position);

private:
    QString m_title;
    QString m_serial;
    QList<CompLibNode *> m_children;
    CompLibNode *m_parent;
};


class CompLibModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Roles {
        NodeDataRole   = Qt::UserRole + 1
    };

    explicit CompLibModel(QObject *parent = 0);
    ~CompLibModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    bool hasChildren(const QModelIndex &parent) const;

    bool insertRow(int row, const QModelIndex &parent);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool dropMimeData(const QMimeData *data, Qt::DropAction dropAction, int, int, const QModelIndex &parent);
    QMimeData *mimeData(const QModelIndexList &indexes) const;

    Qt::DropActions supportedDropActions() const { return Qt::CopyAction | Qt::MoveAction; }

    QStringList mimeTypes() const { QStringList types; types << "text/plain"; return types; }

    inline CompLibNode *node(const QModelIndex &index) const
    {
        if (index.isValid())
            return static_cast<CompLibNode *>(index.internalPointer());
        else
            return m_rootNode;
    }

signals:
    void dataDropped(QString);

private:
    CompLibNode *const m_rootNode;
};

#endif // COMPLIBMODEL_H

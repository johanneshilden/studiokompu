#ifndef LIBINSERTNODECOMMAND_H
#define LIBINSERTNODECOMMAND_H

#include <QUndoCommand>
#include <QPersistentModelIndex>

class QAbstractItemModel;
class CompLibModel;
class CompLibNode;

class LibInsertNodeCommand : public QUndoCommand
{
public:
    LibInsertNodeCommand(CompLibModel *model, QString data, QString title, const QModelIndex &index, int row, QUndoCommand *parent = 0);

    void undo();
    void redo();

private:
    QPersistentModelIndex m_index;
    QString               m_data;
    QString               m_title;
    CompLibModel   *const m_model;
    int                   m_row;
};

#endif // LIBINSERTNODECOMMAND_H

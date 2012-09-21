#include <QDebug>
#include "libinsertnodecommand.h"
#include "complibmodel.h"
#include "complibwidget.h"
#include "compnodeitem.h"

LibInsertNodeCommand::LibInsertNodeCommand(CompLibModel *model, QString data, QString title, const QModelIndex &index, int row, QUndoCommand *parent)
    : QUndoCommand(parent),
      m_index(index),
      m_data(data),
      m_title(title),
      m_model(model),
      m_row(row)
{
    setText(QObject::tr("Insert node into library"));
}

void LibInsertNodeCommand::undo()
{
    m_model->removeRow(m_row, m_index);
}

void LibInsertNodeCommand::redo()
{
    m_model->insertRow(m_row, m_index);
    QModelIndex index = m_model->index(m_row, 0, m_index);
    m_model->setData(index, m_title);
    m_model->setData(index, m_data, CompLibModel::NodeDataRole);
}

#include <QUndoStack>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QDebug>
#include <cassert>
#include "complibwidget.h"
#include "complibmodel.h"
#include "compnodeitem.h"
#include "commands/libinsertnodecommand.h"
#include "lib/comp_serialize.h"

LibNodeDialog::LibNodeDialog(QWidget *parent)
    : QDialog(parent),
      m_edit(new QLineEdit)
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);
    QDialogButtonBox *bbox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                  QDialogButtonBox::Cancel);

    connect(bbox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(bbox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));

    layout->addWidget(m_edit);
    layout->addWidget(bbox);

    setWindowTitle(tr("Node name"));
}

QString LibNodeDialog::text() const
{
    return m_edit->text();
}

CompLibWidget::CompLibWidget(QUndoStack *undoStack, QWidget *parent)
    : QTreeView(parent),
      m_model(new CompLibModel(this)),
      m_undoStack(undoStack)
{
    setModel(m_model);
}

void CompLibWidget::insertNodeItem(CompNodeItem *item)
{
    // index under which the node will be inserted
    QModelIndex parent = QModelIndex();

    CompLibNode *node = m_model->node(parent);
    assert(node);

    LibNodeDialog dialog;
    if (QDialog::Accepted == dialog.exec()) {
        struct buf *buf = buf_new(64);
        node_serialize(item->node(), buf);
        buf_nullterm(buf);

        m_undoStack->push(new LibInsertNodeCommand(m_model,
                                                   QString(buf->data),
                                                   dialog.text(),
                                                   parent,
                                                   node->childCount()));
        buf_destroy(buf);
    }
}

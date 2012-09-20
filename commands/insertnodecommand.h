#ifndef INSERTNODECOMMAND_H
#define INSERTNODECOMMAND_H

#include <QUndoCommand>

class CompNodeItem;
class CompWidget;

class InsertNodeCommand : public QUndoCommand
{
public:
    InsertNodeCommand(CompWidget *widget, CompNodeItem *item, CompNodeItem *parentItem, int n, QUndoCommand *parent = 0);
    ~InsertNodeCommand();

    void undo();
    void redo();

private:
    void rebuild();

    CompWidget *const m_widget;
    CompNodeItem *m_item;
    CompNodeItem *m_parent;
    CompNodeItem *m_tmp;
    struct node_composition *m_comp;
    int m_pos;
};

#endif // INSERTNODECOMMAND_H

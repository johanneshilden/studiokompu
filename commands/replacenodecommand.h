#ifndef REPLACENODECOMMAND_H
#define REPLACENODECOMMAND_H

#include <QUndoCommand>

class CompNodeItem;
class CompWidget;

class ReplaceNodeCommand : public QUndoCommand
{
public:
    ReplaceNodeCommand(CompWidget *widget, CompNodeItem *old, CompNodeItem *item, QUndoCommand *parent = 0);
    ~ReplaceNodeCommand();

    void undo();
    void redo();

private:
    CompNodeItem *switchNodes(CompNodeItem *old, CompNodeItem *neue);

    CompWidget *const m_widget;
    CompNodeItem *m_old;
    CompNodeItem *m_new;
    CompNodeItem *m_item;
};

#endif // REPLACENODECOMMAND_H

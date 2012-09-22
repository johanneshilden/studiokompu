#ifndef SETPROJECTIONCOORDCOMMAND_H
#define SETPROJECTIONCOORDCOMMAND_H

#include <QUndoCommand>

class CompNodeItem;

class SetProjectionCoordCommand : public QUndoCommand
{
public:
    SetProjectionCoordCommand(CompNodeItem *item, int place, QUndoCommand *parent = 0);
    ~SetProjectionCoordCommand();

    void undo();
    void redo();

private:
    CompNodeItem           *const m_item;
    struct node_projection *const m_proj;
    int m_place;
    int m_old;
};

#endif // SETPROJECTIONCOORDCOMMAND_H

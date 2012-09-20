#include <QGraphicsScene>
#include "compnodeitem.h"
#include "setprojectioncoordcommand.h"

SetProjectionCoordCommand::SetProjectionCoordCommand(CompNodeItem *item, int place, QUndoCommand *parent)
    : QUndoCommand(parent),
      m_item(item),
      m_proj((struct node_projection *) item->node()->data),
      m_place(place),
      m_old(m_proj->place)
{
    setText(QObject::tr("Set projection node coordinate"));
}

SetProjectionCoordCommand::~SetProjectionCoordCommand()
{
}

void SetProjectionCoordCommand::undo()
{
    m_proj->place = m_old;
    m_item->scene()->update();
}

void SetProjectionCoordCommand::redo()
{
    m_proj->place = m_place;
    m_item->scene()->update();
}

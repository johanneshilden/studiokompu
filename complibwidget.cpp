#include "complibwidget.h"
#include "complibmodel.h"

CompLibWidget::CompLibWidget(QWidget *parent)
    : QTreeView(parent)
{
    CompLibModel *model = new CompLibModel(this);
    setModel(model);
}

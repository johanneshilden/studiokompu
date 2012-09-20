#ifndef COMPLIBWIDGET_H
#define COMPLIBWIDGET_H

#include <QTreeView>

class CompLibWidget : public QTreeView
{
    Q_OBJECT

public:
    explicit CompLibWidget(QWidget *parent = 0);
};

#endif // COMPLIBWIDGET_H

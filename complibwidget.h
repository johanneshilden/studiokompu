#ifndef COMPLIBWIDGET_H
#define COMPLIBWIDGET_H

#include <QDialog>
#include <QTreeView>

class QLineEdit;

class LibNodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LibNodeDialog(QWidget *parent = 0);
    QString text() const;

private:
    QLineEdit *const m_edit;
};

class QUndoStack;
class CompLibModel;
class CompNodeItem;

class CompLibWidget : public QTreeView
{
    Q_OBJECT

public:
    explicit CompLibWidget(QUndoStack *undoStack, QWidget *parent = 0);
    void insertNodeItem(CompNodeItem *item);

private:
    CompLibModel *const m_model;
    QUndoStack   *const m_undoStack;
};

#endif // COMPLIBWIDGET_H

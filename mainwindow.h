#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class QUndoStack;
class CompNodeItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void updateActions(CompNodeItem *selectedItem);

private:
    void disableAllActions();

    QToolBar *const  m_toolBar;
    QList<QAction *> m_nodeActions;
    QAction *m_insertLegAction;
    QAction *m_coordinateAction;
    QAction *m_debugDumpAction;
    QUndoStack *m_undoStack;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class CompNodeItem;
class CompWidget;
class CompLibWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void updateActions(CompNodeItem *selectedItem);
    void clearUndoStack();
    void addFuncToLibrary();
    void insertFuncFromLib();

private:
    void disableAllActions();

    QToolBar   *const m_toolBar;
    CompWidget *const m_compWidget;
    CompLibWidget *const m_libWidget;
    QList<QAction *>  m_nodeActions;
    QAction *m_insertLegAction;
    QAction *m_coordinateAction;
};

#endif // MAINWINDOW_H

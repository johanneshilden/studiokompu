#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class QActionGroup;
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
};

#endif // MAINWINDOW_H

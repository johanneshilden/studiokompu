#include <QDebug>
#include <QToolBar>
#include <QAction>
#include "mainwindow.h"
#include "compwidget.h"
#include "compnodeitem.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_toolBar(addToolBar("default"))
{
    CompWidget *widget = new CompWidget;
    setCentralWidget(widget);

    //

    m_toolBar->setContextMenuPolicy(Qt::CustomContextMenu);
    m_toolBar->setMovable(false);

    QAction *zeroAction = m_toolBar->addAction("Z");
    QAction *projAction = m_toolBar->addAction("P");
    QAction *succAction = m_toolBar->addAction("+");
    QAction *compAction = m_toolBar->addAction("C");
    QAction *recuAction = m_toolBar->addAction("R");
    QAction *srchAction = m_toolBar->addAction("S");
    QAction *invlAction = m_toolBar->addAction("-");
    m_toolBar->addSeparator();
    m_insertLegAction  = m_toolBar->addAction("|");
    m_coordinateAction = m_toolBar->addAction("X");
    m_toolBar->addSeparator();
    m_debugDumpAction  = m_toolBar->addAction("dmp");

    m_nodeActions.push_back(zeroAction);
    m_nodeActions.push_back(projAction);
    m_nodeActions.push_back(succAction);
    m_nodeActions.push_back(compAction);
    m_nodeActions.push_back(recuAction);
    m_nodeActions.push_back(srchAction);
    m_nodeActions.push_back(invlAction);

    connect(zeroAction, SIGNAL(triggered()), widget, SLOT(insertZeroNode()));
    connect(projAction, SIGNAL(triggered()), widget, SLOT(insertProjectionNode()));
    connect(succAction, SIGNAL(triggered()), widget, SLOT(insertSuccNode()));
    connect(compAction, SIGNAL(triggered()), widget, SLOT(insertCompositionNode()));
    connect(recuAction, SIGNAL(triggered()), widget, SLOT(insertRecursionNode()));
    connect(srchAction, SIGNAL(triggered()), widget, SLOT(insertSearchNode()));
    connect(invlAction, SIGNAL(triggered()), widget, SLOT(insertInvalidNode()));

    connect(m_insertLegAction, SIGNAL(triggered()), widget, SLOT(insertNodeLeg()));
    connect(m_coordinateAction, SIGNAL(triggered()), widget, SLOT(showProjectionDialog()));
    connect(m_debugDumpAction, SIGNAL(triggered()), widget, SLOT(dumpNode()));

    connect(widget, SIGNAL(selectionChanged(CompNodeItem *)), this, SLOT(updateActions(CompNodeItem *)));

    updateActions(0);


    //
}

MainWindow::~MainWindow()
{    
}

void MainWindow::updateActions(CompNodeItem *selectedItem)
{
    if (!selectedItem) {
        disableAllActions();
        return;
    }

    m_debugDumpAction->setEnabled(true);

    QList<QAction *>::const_iterator i = m_nodeActions.constBegin();
    for (; i != m_nodeActions.constEnd(); ++i)
        (*i)->setEnabled(true);

    const CompNodeItem::NodeType type = selectedItem->nodeType();
    m_coordinateAction->setEnabled(CompNodeItem::ProjectionNode == type);
    m_insertLegAction->setEnabled(CompNodeItem::CompositionNode == type);
}

void MainWindow::disableAllActions()
{
    QList<QAction *> actions = m_toolBar->actions();
    QList<QAction *>::const_iterator i;
    for (i = actions.constBegin(); i != actions.constEnd(); ++i) {
        QAction *action = *i;
        action->setEnabled(false);
    }
}

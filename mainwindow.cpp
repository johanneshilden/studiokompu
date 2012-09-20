#include <QDebug>
#include <QToolBar>
#include <QAction>
#include <QUndoStack>
#include <QHBoxLayout>
#include "mainwindow.h"
#include "compwidget.h"
#include "compnodeitem.h"
#include "complibwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_toolBar(addToolBar("default")),
      m_compWidget(new CompWidget),
      m_libWidget(new CompLibWidget)
{
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    widget->setLayout(layout);
    layout->addWidget(m_compWidget);
    layout->addWidget(m_libWidget);

    setCentralWidget(widget);

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
    m_toolBar->addSeparator();
    QAction *undoAction = m_toolBar->addAction(tr("Undo"));
    QAction *redoAction = m_toolBar->addAction(tr("Redo"));
    QAction *clearStackAction = m_toolBar->addAction("Clear stack");
    m_toolBar->addSeparator();
    QAction *addToLibAction = m_toolBar->addAction(tr("Add to library"));

    m_nodeActions.push_back(zeroAction);
    m_nodeActions.push_back(projAction);
    m_nodeActions.push_back(succAction);
    m_nodeActions.push_back(compAction);
    m_nodeActions.push_back(recuAction);
    m_nodeActions.push_back(srchAction);
    m_nodeActions.push_back(invlAction);

    connect(zeroAction, SIGNAL(triggered()), m_compWidget, SLOT(insertZeroNode()));
    connect(projAction, SIGNAL(triggered()), m_compWidget, SLOT(insertProjectionNode()));
    connect(succAction, SIGNAL(triggered()), m_compWidget, SLOT(insertSuccNode()));
    connect(compAction, SIGNAL(triggered()), m_compWidget, SLOT(insertCompositionNode()));
    connect(recuAction, SIGNAL(triggered()), m_compWidget, SLOT(insertRecursionNode()));
    connect(srchAction, SIGNAL(triggered()), m_compWidget, SLOT(insertSearchNode()));
    connect(invlAction, SIGNAL(triggered()), m_compWidget, SLOT(insertInvalidNode()));

    connect(m_insertLegAction, SIGNAL(triggered()), m_compWidget, SLOT(insertNodeLeg()));
    connect(m_coordinateAction, SIGNAL(triggered()), m_compWidget, SLOT(showProjectionDialog()));
    connect(m_debugDumpAction, SIGNAL(triggered()), m_compWidget, SLOT(dumpNode()));

    connect(undoAction, SIGNAL(triggered()), m_compWidget->undoStack(), SLOT(undo()));
    connect(redoAction, SIGNAL(triggered()), m_compWidget->undoStack(), SLOT(redo()));
    connect(clearStackAction, SIGNAL(triggered()), this, SLOT(clearUndoStack()));

    connect(m_compWidget, SIGNAL(selectionChanged(CompNodeItem *)), this, SLOT(updateActions(CompNodeItem *)));

    connect(addToLibAction, SIGNAL(triggered()), this, SLOT(addFuncToLibrary()));

    updateActions(0);
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

void MainWindow::clearUndoStack()
{
    m_compWidget->undoStack()->clear();
}

void MainWindow::addFuncToLibrary()
{
    CompNodeItem *node = m_compWidget->topNode();
    CompNodeItem *copy = new CompNodeItem(node_clone(node->node()));

    //

    CompLibNode *item1 = new CompLibNode("item x");
    CompNodeItem *tn = m_compWidget->topNode();
    tn->insert(tn->childCount(), item1);
}

void MainWindow::disableAllActions()
{
    QList<QAction *> actions = m_toolBar->actions();
    QList<QAction *>::const_iterator i;
    for (i = actions.constBegin(); i != actions.constEnd() - 3; ++i)
        (*i)->setEnabled(false);
}

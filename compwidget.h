#ifndef COMPWIDGET_H
#define COMPWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QGraphicsView>

#include <QDebug>
#include <QDragEnterEvent>

class QSpinBox;

class CompProjectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CompProjectionDialog(int place, QWidget *parent = 0);
    int val() const;

private:
    QSpinBox *const m_sb;
};

class CompGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit CompGraphicsView(QWidget *parent = 0);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void dataDropped(QString);

private:
    QPoint m_dragStartPosition;
};

class QPushButton;
class QLineEdit;
class QPlainTextEdit;
class QGraphicsView;
class QUndoStack;
class CompGraphicsScene;
class CompNodeItem;

class CompWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompWidget(QWidget *parent = 0);
    ~CompWidget();

    inline QUndoStack *undoStack() const { return m_undoStack; }
    inline void setTopNode(CompNodeItem *node) { m_topNode = node; validate(); }
    inline CompNodeItem *topNode() const { return m_topNode; }
    void validate();

    void replaceSelectedNode(struct node *node);
    CompNodeItem *selectedNodeItem() const;

protected:
    bool eventFilter(QObject *object, QEvent *event);

signals:
    void selectionChanged(CompNodeItem *);
    void validStateChanged(bool);

protected slots:
    void insertNodeFromData(QString data);
    void insertZeroNode();
    void insertSuccNode();
    void insertInvalidNode();
    void insertCompositionNode();
    void insertRecursionNode();
    void insertProjectionNode();
    void insertSearchNode();
    void insertNodeLeg();
    void dumpNode();

    void compute();
    void showProjectionDialog();

    void emitSelectionChanged();

private:
    void initUi();

    CompGraphicsScene *const m_scene;
    CompGraphicsView  *const m_view;
    QPushButton       *const m_computeButton;
    QLineEdit         *const m_input;
    QPlainTextEdit    *const m_logWindow;
    QUndoStack        *const m_undoStack;
    CompNodeItem      *m_topNode;
    bool               m_valid;
};

#endif // COMPWIDGET_H

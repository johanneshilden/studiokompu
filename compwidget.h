#ifndef COMPWIDGET_H
#define COMPWIDGET_H

#include <QWidget>
#include <QDialog>

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

class CompGraphicsScene;
class QPushButton;
class QLineEdit;
class QPlainTextEdit;
class QUndoStack;
class CompNodeItem;

class CompWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompWidget(QWidget *parent = 0);
    ~CompWidget();

    inline QUndoStack *undoStack() const { return m_undoStack; }
    inline void setTopNode(CompNodeItem *node) { m_topNode = node; validate(); }
    void validate();

protected:
    bool eventFilter(QObject *object, QEvent *event);

signals:
    void selectionChanged(CompNodeItem *);
    void validStateChanged(bool);

protected slots:
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
    void replaceSelectedNode(struct node *node);

    CompGraphicsScene *const m_scene;
    QPushButton       *const m_computeButton;
    QLineEdit         *const m_input;
    QPlainTextEdit    *const m_logWindow;
    QUndoStack        *const m_undoStack;
    CompNodeItem      *m_topNode;
    bool               m_valid;
};

#endif // COMPWIDGET_H

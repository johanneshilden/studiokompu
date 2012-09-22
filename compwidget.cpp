#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QUndoStack>
#include <QApplication>
#include <QDebug>
#include <QUndoView>
#include "compnodeitem.h"
#include "compgraphicsscene.h"
#include "compwidget.h"
#include "commands/replacenodecommand.h"
#include "commands/insertnodecommand.h"
#include "commands/setprojectioncoordcommand.h"
#include "lib/comp_serialize.h"

CompProjectionDialog::CompProjectionDialog(int place, QWidget *parent)
    : QDialog(parent),
      m_sb(new QSpinBox)
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QDialogButtonBox *bbox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                  QDialogButtonBox::Cancel);

    connect(bbox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(bbox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));

    layout->addWidget(m_sb);
    layout->addWidget(bbox);

    m_sb->setValue(place + 1);
    m_sb->setRange(1, 1000);

    setWindowTitle(tr("Projection coordinate"));
}

int CompProjectionDialog::val() const
{
    return m_sb->value();
}

CompGraphicsView::CompGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setAcceptDrops(true);
}

void CompGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    QGraphicsView::dragEnterEvent(event);
}

void CompGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
//    const QMimeData *mime = event->mimeData();

    CompGraphicsScene *scene = static_cast<CompGraphicsScene *>(this->scene());
    CompNodeItem *nodeItem = qgraphicsitem_cast<CompNodeItem *>(itemAt(event->pos()));
    scene->setDropItem(nodeItem);
    event->setAccepted(nodeItem);
    event->setDropAction(Qt::CopyAction);
    scene->update();
}

void CompGraphicsView::dropEvent(QDropEvent *event)
{
    const QMimeData *mime = event->mimeData();
    emit dataDropped(mime->text());
}

void CompGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_dragStartPosition = event->pos();

    QGraphicsView::mousePressEvent(event);
}

void CompGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton)) {
        if ((event->pos() - m_dragStartPosition).manhattanLength() >=
                QApplication::startDragDistance()) {

            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;

            mimeData->setData("text/plain", "[0,0]");
            drag->setMimeData(mimeData);

            Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);

        }
    }

    QGraphicsView::mouseMoveEvent(event);
}

CompWidget::CompWidget(QWidget *parent)
    : QWidget(parent),
      m_scene(new CompGraphicsScene(this)),
      m_view(new CompGraphicsView),
      m_computeButton(new QPushButton(tr("Compute"))),
      m_input(new QLineEdit),
      m_logWindow(new QPlainTextEdit),
      m_undoStack(new QUndoStack(this)),
      m_topNode(new CompNodeItem(invalid_node_new())),
      m_valid(false)
{
    initUi();
}

CompWidget::~CompWidget()
{
}

void CompWidget::validate()
{
    bool v = m_topNode && m_topNode->isValid();
    if (v != m_valid)
        emit validStateChanged(m_valid = v);
}

void CompWidget::replaceSelectedNode(struct node *node)
{
    CompNodeItem *nodeItem = m_scene->selectedNodeItem();
    if (!nodeItem) {
        node_destroy(node);
        return;
    }
    m_undoStack->push(new ReplaceNodeCommand(this,
                                             nodeItem,
                                             new CompNodeItem(node)));
}

CompNodeItem *CompWidget::selectedNodeItem() const
{
    return m_scene->selectedNodeItem();
}

bool CompWidget::eventFilter(QObject *object, QEvent *event)
{
    if (m_scene == object && QEvent::KeyPress == event->type()) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        CompNodeItem *nodeItem = m_scene->selectedNodeItem();
        if (keyEvent && nodeItem) {
            switch (keyEvent->key())
            {
            case Qt::Key_Up:
            {
                QGraphicsItem *parent = nodeItem->parentItem();
                if (parent) {
                    nodeItem->setSelected(false);
                    parent->setSelected(true);
                }
                break;
            }
            case Qt::Key_Down:
            {
                if (!nodeItem->isLeaf()) {
                    QList<CompNodeItem *> cn = nodeItem->childNodes();
                    if (!cn.isEmpty()) {
                        cn.first()->setSelected(true);
                        nodeItem->setSelected(false);
                    }
                }
                break;
            }
            case Qt::Key_Left:
            {
                CompNodeItem *parentNode = qgraphicsitem_cast<CompNodeItem *>(nodeItem->parentItem());
                if (parentNode) {
                    QList<CompNodeItem *> siblings = parentNode->childNodes();
                    int n = siblings.indexOf(nodeItem);

                    if (n > 0) {
                        siblings.at(n - 1)->setSelected(true);
                        nodeItem->setSelected(false);
                    }
                }
                break;
            }
            case Qt::Key_Right:
            {
                CompNodeItem *parentNode = qgraphicsitem_cast<CompNodeItem *>(nodeItem->parentItem());
                if (parentNode) {
                    QList<CompNodeItem *> siblings = parentNode->childNodes();
                    int n = siblings.indexOf(nodeItem);

                    if (n < siblings.count() - 1) {
                        siblings.at(n + 1)->setSelected(true);
                        nodeItem->setSelected(false);
                    }
                }
                break;
            }
            default:
                break;
            } // end switch
        }
    }
    return QWidget::eventFilter(object, event);
}

void CompWidget::insertNodeFromData(QString data)
{
    QByteArray ba = data.toAscii();
    struct buf *buf = buf_new(1024);
    buf_append_chars(buf, ba.data());
    if (node_serial_data_is_valid(buf) < 0) {
        qWarning() << "Invalid node tree";
        qDebug() << data;
        buf_destroy(buf);
        return;
    }
    foreach (QGraphicsItem *item, m_scene->selectedItems())
        item->setSelected(false);
    CompNodeItem *item = m_scene->dropItem();
    if (item) {
        item->setSelected(true);
        replaceSelectedNode(node_unserialize(buf));
    }
    buf_destroy(buf);
    m_scene->setDropItem(0);
}

void CompWidget::insertZeroNode()
{
    replaceSelectedNode(zero_node_new());
}

void CompWidget::insertSuccNode()
{
    replaceSelectedNode(successor_node_new());
}

void CompWidget::insertInvalidNode()
{
    replaceSelectedNode(invalid_node_new());
}

void CompWidget::insertCompositionNode()
{
    struct node **g = node_array_new(2);
    g[0] = invalid_node_new();
    g[1] = NULL;

    struct node *node = composition_node_new(invalid_node_new(), g);
    replaceSelectedNode(node);
}

void CompWidget::insertRecursionNode()
{
    replaceSelectedNode(recursion_node_new(invalid_node_new(),
                                           invalid_node_new()));
}

void CompWidget::insertProjectionNode()
{
    replaceSelectedNode(projection_node_new(0));
}

void CompWidget::insertSearchNode()
{
    replaceSelectedNode(search_node_new(invalid_node_new()));
}

void CompWidget::insertNodeLeg()
{
    CompNodeItem *nodeItem = m_scene->selectedNodeItem();
    if (!nodeItem || CompNodeItem::CompositionNode != nodeItem->nodeType())
        return;

    m_undoStack->push(new InsertNodeCommand(this,
                                            new CompNodeItem(invalid_node_new()),
                                            nodeItem, 0));
}

void CompWidget::dumpNode()
{
    CompNodeItem *nodeItem = m_scene->selectedNodeItem();
    if (!nodeItem)
        return;
    //
    node_dump(nodeItem->node());
}

void CompWidget::compute()
{
    QStringList arglist = m_input->text().split(',');
    QList<int> args;
    QStringList::const_iterator i;
    for (i = arglist.constBegin(); i != arglist.constEnd(); ++i)
        args.push_back((*i).toInt());
    m_logWindow->setPlainText(QString::number(m_topNode->compute(args)));
}

void CompWidget::showProjectionDialog()
{
    CompNodeItem *nodeItem = m_scene->selectedNodeItem();
    if (!nodeItem || CompNodeItem::ProjectionNode != nodeItem->nodeType())
        return;

    struct node_projection *proj = (struct node_projection *) nodeItem->node()->data;
    CompProjectionDialog dialog(proj->place);
    if (QDialog::Accepted == dialog.exec())
        m_undoStack->push(new SetProjectionCoordCommand(nodeItem,
                                                        dialog.val() - 1));
}

void CompWidget::emitSelectionChanged()
{
    emit selectionChanged(m_scene->selectedNodeItem());
}

void CompWidget::initUi()
{
    m_scene->addItem(m_topNode);
    m_scene->installEventFilter(this);

    m_view->setScene(m_scene);
    m_view->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(m_input);
    inputLayout->addWidget(m_computeButton);

    m_logWindow->setFont(QFont("monospace"));
    m_logWindow->setReadOnly(true);
    m_logWindow->setMaximumHeight(80);
    m_logWindow->setStyleSheet("QPlainTextEdit { background: #000; color: #0f0; }");

    layout->addLayout(inputLayout);
    layout->addWidget(m_logWindow);
    layout->addWidget(m_view);

    /*
    --
    */
    QUndoView *uv = new QUndoView(m_undoStack);
    uv->setMaximumHeight(80);
    layout->addWidget(uv);

    setMinimumSize(300, 300);

    connect(m_computeButton, SIGNAL(clicked()), this, SLOT(compute()));
    connect(m_scene, SIGNAL(selectionChanged()), this, SLOT(emitSelectionChanged()));
    connect(this, SIGNAL(validStateChanged(bool)), m_computeButton, SLOT(setEnabled(bool)));
    connect(m_view, SIGNAL(dataDropped(QString)), this, SLOT(insertNodeFromData(QString)));

    emit validStateChanged(m_topNode && m_topNode->isValid());
}

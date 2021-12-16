#include "FlowWidget.hpp"

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QLibrary>
#include <QMenu>

#include "Types.hpp"
#include "Logger.hpp"
// I/O Operations
#include "FlowItems/FlowItem_Import.hpp"
#include "FlowItems/FlowItem_Edit.hpp"
#include "FlowItems/FlowItem_Export.hpp"
// Layer Operations
#include "FlowItems/FlowItem_Map.hpp"
#include "FlowItems/FlowItem_Extract.hpp"
// Layout Operations
#include "FlowItems/FlowItem_AND.hpp"
#include "FlowItems/FlowItem_OR.hpp"
#include "FlowItems/FlowItem_Heal.hpp"

FlowScene::FlowScene(QWidget *parent, Logger *logger) : p_logger(logger) {
  connect(this, SIGNAL(selectionChanged()), this, SLOT(OnSelectionChanged()));
}

void FlowScene::OnSelectionChanged() {
  QString text;

  foreach(QGraphicsItem * item, selectedItems()) {
    if (item->type() != QGraphicsItem::UserType + 1)
      continue;
    text = static_cast<FlowItem *>(item)->GetInfoString();
  }
  p_logger->ShowInfoString(text);
}

void FlowScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
  if (event->mimeData()->hasFormat("application/x-dndflowitemdata")) {
    if (event->source() == (QObject*)this) {
      event->setDropAction(Qt::MoveAction);
      event->accept();
    }
    else
      event->acceptProposedAction();
  }
  else
    event->ignore();
}

FlowWidget::FlowWidget(QWidget *parent, Logger *logger, UserFlowItemsManager *manager) : QGraphicsView(parent), p_logger(logger), p_scene(nullptr), p_manager(manager), p_eventFilter(nullptr) {
  setAcceptDrops(true);

  p_scene = new FlowScene(this, p_logger);
  setScene(p_scene);
  p_scene->setSceneRect(QRectF(0.0, 0.0, 200.0, 200.0));

  setBackgroundBrush(QPixmap(":/other/background.png"));

  p_eventFilter = new FlowSceneEventFilter(this, p_logger);
  p_eventFilter->AssignGraphicsScene(p_scene);

  setDragMode(QGraphicsView::RubberBandDrag);
  
  centerPoint = mapToScene(geometry().center());
}

FlowWidget::~FlowWidget() {
  p_scene->clearSelection();

  p_scene->removeEventFilter(p_eventFilter);
  p_scene->clear();
  delete p_scene;
  p_scene = nullptr;
  delete p_eventFilter;
  p_eventFilter = nullptr;
}

void FlowWidget::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasFormat("application/x-dndflowitemdata")) {
    if (event->source() == (QObject *)this) {
      event->setDropAction(Qt::MoveAction);
      event->accept();
    }
    else
      event->acceptProposedAction();
  }
  else
    event->ignore();
}

void FlowWidget::dropEvent(QDropEvent *event) {

  if (!event->mimeData()->hasFormat("application/x-dndflowitemdata")) {
    event->ignore();
    return;
  }

  QByteArray itemData = event->mimeData()->data("application/x-dndflowitemdata");
  QDataStream dataStream(&itemData, QIODevice::ReadOnly);

  FlowItemType itemType = FlowItemType::undefined;
  int val = 0;
  int userItemId = 0;
  dataStream >> val;
  itemType = static_cast<FlowItemType>(val);
  dataStream >> userItemId;

  FlowItem  *p_item = nullptr;

  switch (itemType) {
    // I/O Operations
    case FlowItemType::importdata:
      p_item = new FlowItem_Import(p_logger);
      break;
    case FlowItemType::editdata:
      p_item = new FlowItem_Edit(p_logger);
      break;
    case FlowItemType::exportdata:
      p_item = new FlowItem_Export(p_logger);
      break;
    // Layer Operations
    case FlowItemType::map:
      p_item = new FlowItem_Map(p_logger);
      break;
    case FlowItemType::extract:
      p_item = new FlowItem_Extract(p_logger);
      break;
    // Layout Operations
    case FlowItemType::AND:
      p_item = new FlowItem_AND(p_logger);
      break;
    case FlowItemType::OR:
      p_item = new FlowItem_OR(p_logger);
      break;
    case FlowItemType::heal:
      p_item = new FlowItem_Heal(p_logger);
      break;
    // All other cases
    case FlowItemType::userdefined: {
      if (userItemId < 4131 || userItemId > 4141) {
        p_logger->Error("User-defined flow item has incorrect ID.");
        break;
      }
      userItemId -= 4131;
      p_item = p_manager->userFlowItems[userItemId]->GetFlowItem();
      break;
    }
    default:
      p_logger->Error("Unsupported item type was dropped");
      event->ignore();
      return;
  }

  if (nullptr == p_item) {
    event->ignore();
    return;
  }

  p_scene->addItem(p_item);
  p_item->setPos(mapToScene(event->pos()));

  if (!p_item->DropEventHandler()) {
    p_scene->removeItem(p_item);
    delete p_item;
    p_item = nullptr;
    event->ignore();
    return;
  }

  if (event->source() == this) {
    event->setDropAction(Qt::MoveAction);
    event->accept();
  }
  else {
    event->acceptProposedAction();
  }
}

QList <FlowItem *>FlowWidget::GetSelectedItems() {
  QList<FlowItem *> retValue;
  
  foreach(QGraphicsItem *item, p_scene->selectedItems()) {
    if (item->type() != QGraphicsItem::UserType + 1)
      continue;
    retValue.push_back(static_cast<FlowItem *>(item));
  }

  //TODO: Return a pointer to a list? Return reference to a list?
  return retValue;
}

void FlowWidget::resizeEvent(QResizeEvent *event) {
  centerOn(centerPoint);
}

QGraphicsItem *FlowWidget::GetItemAtXY(const QPointF &pos) {
  QList<QGraphicsItem*> items = p_scene->items(QRectF(pos - QPointF(1, 1), QSize(3, 3)));

  foreach(QGraphicsItem * p_item, items)
    if (p_item->type() > QGraphicsItem::UserType)
      return p_item;

  return nullptr;
}

void FlowWidget::contextMenuEvent(QContextMenuEvent *event) {
  QGraphicsItem *p_item = p_scene->focusItem();
  if (!p_item)
    return;
  if (p_item->type() != FlowItem::Type)
    return;

  FlowItem *p_flowItem = static_cast<FlowItem *>(p_item);
  p_flowItem->setSelected(true);

  QMenu contextMenu;
  QAction *actRunTo = contextMenu.addAction("Execute Item");
  actRunTo->setEnabled(false);
  QAction *actReset = contextMenu.addAction("Reset");
  if (p_flowItem->GetStatus() != FlowItemStatus::completed)
    actReset->setEnabled(false);
  QAction *actViewExternal = contextMenu.addAction("View in external viewer");
  contextMenu.addSeparator();
  QAction *actDisconnect = contextMenu.addAction("Disconnect");
  QAction *actComment = contextMenu.addAction("Comment");
  actComment->setEnabled(false);
  contextMenu.addSeparator();
  QAction *actDelete = contextMenu.addAction("Delete item");
  
  QAction *result = contextMenu.exec(event->globalPos());

  if (result == actRunTo) {
  }
  if (result == actReset) {
    p_flowItem->OnHandleEvent_Reset();
    return;
  }
  if (result == actViewExternal) {
  }
  if (result == actDisconnect) {
    p_flowItem->Disconnect();
    return;
  }
  if (result == actComment) {
  }
  if (result == actDelete) {
    p_scene->removeItem(p_flowItem);
    delete p_flowItem;
    p_flowItem = nullptr;
    return;
  }

}

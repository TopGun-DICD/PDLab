#include "FlowWidget.hpp"

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QLibrary>

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
}

FlowWidget::~FlowWidget() {
  //TODO: Вот тут нужно сделать так, чтобы все элементы p_scene стали unselected, иначе вылетает лог
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
    case FlowItemType::heal:
      p_item = new FlowItem_Heal(p_logger);
      break;
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



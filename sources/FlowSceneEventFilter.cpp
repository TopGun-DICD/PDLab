#include "FlowSceneEventFilter.hpp"

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>

#include "FlowItems/FlowItemConnection.hpp"
#include "FlowItems/FlowItemPort.hpp"
#include "FlowItems/FlowItem.hpp"
#include "Logger.hpp"

FlowSceneEventFilter::FlowSceneEventFilter(QWidget *parent, BasicLogger *logger) : QObject(parent), p_parentView(static_cast<QGraphicsView *>(parent)), p_scene(nullptr), p_connection(nullptr), p_logger(logger), zIndex(0.0) {
}

void FlowSceneEventFilter::AssignGraphicsScene(QGraphicsScene *scene) {
  p_scene = scene;
  if (!p_scene)
    return;
  p_scene->installEventFilter(this);
}

QGraphicsItem *FlowSceneEventFilter::GetItemAtXY(const QPointF &pos) {
  QList<QGraphicsItem*> items = p_scene->items(QRectF(pos - QPointF(1, 1), QSize(3, 3)));

  foreach(QGraphicsItem *p_item, items)
    if (p_item->type() > QGraphicsItem::UserType)
      return p_item;

  return nullptr;
}

bool FlowSceneEventFilter::eventFilter(QObject *object, QEvent *event) {
  switch (event->type()) {
    case QEvent::GraphicsSceneMousePress:
      return OnMousePress(object, static_cast<QGraphicsSceneMouseEvent *>(event));
    case QEvent::GraphicsSceneMouseRelease:
      return OnMouseRelease(object, static_cast<QGraphicsSceneMouseEvent *>(event));
    case QEvent::GraphicsSceneMouseMove:
      return OnMouseMove(object, static_cast<QGraphicsSceneMouseEvent *>(event));
    case QEvent::KeyPress:
      return OnKeyPress(object, static_cast<QKeyEvent *>(event));
  }
  return QObject::eventFilter(object, event);
}

bool FlowSceneEventFilter::OnMousePress(QObject *object, QGraphicsSceneMouseEvent *event) {
  QGraphicsItem *p_item = nullptr;

  switch (event->button()) {
    case Qt::LeftButton:
      p_item = GetItemAtXY(event->scenePos());
      if (!p_item)
        return QObject::eventFilter(object, event);

      if (p_item->type() == FlowItemPort::Type) {

        FlowItemPort *p_port = static_cast<FlowItemPort *>(p_item);
        if (p_port->direction == PortDirection::input && !p_port->connections.empty()) {
          p_logger->Error("You can't join to the input port more than one connection.");
          return true;
        }

        p_parentView->setDragMode(QGraphicsView::NoDrag);

        p_connection = new FlowItemConnection;
        p_scene->addItem(p_connection);
        p_connection->AssignPortA(p_port);
        p_connection->SetPositionA(p_item->scenePos());
        p_connection->SetPositionB(event->scenePos());
        p_connection->UpdatePath();
        p_item->update();
        return true;
      }
      if (p_item->type() == FlowItem::Type) {
        zIndex += 0.01;
        p_item->setZValue(zIndex);
        //p_logger->Log(QString("Z-Index was set to %1").arg(zIndex));
      }
      break;
    /*
    case Qt::RightButton:
      p_item = GetItemAtXY(event->scenePos());
      if (p_item && (p_item->type() == FlowItemConnection::Type || p_item->type() == FlowItem::Type)) {
        delete p_item;
        p_item = nullptr;
      }
      break;
    */

  }
  return QObject::eventFilter(object, event);
}

bool FlowSceneEventFilter::OnMouseMove(QObject *object, QGraphicsSceneMouseEvent *event) {
  if (!p_connection)
    return QObject::eventFilter(object, event);
  p_connection->SetPositionB(event->scenePos());
  p_connection->UpdatePath();
  return true;
}

bool FlowSceneEventFilter::OnMouseRelease(QObject *object, QGraphicsSceneMouseEvent *event) {
  QGraphicsItem *p_item = nullptr;

  p_parentView->setDragMode(QGraphicsView::RubberBandDrag);

  if (!p_connection || event->button() != Qt::LeftButton)
    return QObject::eventFilter(object, event);

  p_item = GetItemAtXY(event->scenePos());

  if (p_item && p_item->type() == FlowItemPort::Type) {
    FlowItemPort *p_portA = p_connection->p_portA;
    FlowItemPort *p_portB = static_cast<FlowItemPort *>(p_item);

    if (p_portB->direction == PortDirection::input && !p_portB->connections.empty()) {
      p_logger->Error("You can't join to the input port more than one connection.");
      delete p_connection;
      p_connection = nullptr;
      return true;
    }

    if (p_portA->p_owner == p_portB->p_owner) {
      p_logger->Error("You can't connect input and output of the same item.");
      delete p_connection;
      p_connection = nullptr;
      return true;
    }

    if (p_portA->direction == p_portB->direction) {
      p_logger->Error("You can't connect two ports of the same direction.");
      delete p_connection;
      p_connection = nullptr;
      return true;
    }

    if (p_portA->IsConnectedWith(p_portB)) {
      p_logger->Error("These ports are already connected.");
      delete p_connection;
      p_connection = nullptr;
      return true;
    }

    p_connection->SetPositionB(p_portB->scenePos());
    p_connection->AssignPortB(p_portB);
    p_connection->UpdatePath();
    p_item->update();

    if (p_connection->GetPortA()->GetOwner()->GetStatus() == FlowItemStatus::completed) {
      p_connection->GetPortB()->AssignLayout(p_connection->GetPortA()->GetLayout());
    }

    p_logger->Log(QString("Two blocks are connected: '%1' and '%2'").arg(p_connection->GetPortA()->GetOwner()->GetItemTypeAsString(),
                                                                         p_connection->GetPortB()->GetOwner()->GetItemTypeAsString()));
    
    p_connection = nullptr;
    return true;
  }

  p_logger->Warning("You can connect only ports.");
  delete p_connection;
  p_connection = nullptr;
  return true;
}

bool FlowSceneEventFilter::OnKeyPress(QObject *object, QKeyEvent *event) {
  QList<QGraphicsItem *> selectedItems;

  switch (event->key()) {
    case Qt::Key_F5:
      //p_logger->Log("Running the flow");
      break;
    case Qt::Key_Delete:
      selectedItems = p_scene->selectedItems();
      if (selectedItems.empty())
        break;
      //p_logger->Log("Deleting items");
      foreach(QGraphicsItem * item, selectedItems)
        delete item;
      break;
  }
  return true;
}


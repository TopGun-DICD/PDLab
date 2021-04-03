#include "FlowItem.hpp"

#pragma warning(disable: 26451)
#pragma warning(disable: 26812)

#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent> 
#include <QMenu>

#include <ctime>

#include "FlowItemConnection.hpp"
#include "LayoutLoader.hpp"

#define ITEM_WIDTH  140
#define ITEM_HEIGHT 50

QPoint pointBtnRun(ITEM_WIDTH/2 - 2 * (2 + 16), -5);
QPoint pointBtnOptions(ITEM_WIDTH / 2 - 1 * (2 + 16), -5);

FlowItem::FlowItem(FlowItemType type, QString title, BasicLogger *logger, LayoutOwnershipMode mode = LayoutOwnershipMode::make_nothing) : QGraphicsPathItem(nullptr), itemType(type), titleString(title), itemStatus(FlowItemStatus::unknown), p_logger(logger), layoutOwnershipMode(mode), p_resultLayout(nullptr) {
  pathShape.addRoundedRect(QRectF(-ITEM_WIDTH / 2, -ITEM_HEIGHT / 2, ITEM_WIDTH, ITEM_HEIGHT), 5, 5);
  setPath(pathShape);

  pathHeader.addRoundedRect(QRectF(-ITEM_WIDTH / 2, -ITEM_HEIGHT / 2, ITEM_WIDTH, ITEM_HEIGHT / 3), 5, 5);

  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemIsFocusable);
  setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

FlowItem::~FlowItem() {
  Disconnect();

  switch (layoutOwnershipMode) {
    case LayoutOwnershipMode::make_new:
    case LayoutOwnershipMode::make_copy:
      if (p_resultLayout) {
        delete p_resultLayout;
        p_resultLayout = nullptr;
      }
      break;
  }
}

void FlowItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)

  if (isSelected()) {
    painter->setPen(QPen(QColor(255, 255, 255)));
    painter->setBrush(QBrush(QColor(152, 152, 152)));
  }
  else {
    painter->setPen(QPen(QColor(50, 54, 68)));
    painter->setBrush(QBrush(QColor(152, 152, 152)));
  }

  painter->drawPath(path());

  painter->setBrush(QBrush(titleBgColor));
  painter->drawPath(pathHeader);
  painter->setPen(QPen(Qt::white));
  painter->drawText(pathHeader.boundingRect(), Qt::AlignCenter, titleString);

  painter->setPen(QPen(Qt::black));
  painter->drawText(QPoint(-boundingRect().width() / 2 + 10, 5), topString);
  painter->drawText(QPoint(-boundingRect().width() / 2 + 10, 20), bottomString);

  switch (itemStatus) {
    case FlowItemStatus::completed:
      painter->drawPixmap(pointBtnRun, QPixmap(":/flowitembuttons/view.png"));
      break;
    default:
      painter->drawPixmap(pointBtnRun, QPixmap(":/flowitembuttons/run.png"));
      break;
  }
  
  painter->drawPixmap(pointBtnOptions, QPixmap(":/flowitembuttons/settings.png"));
}

bool FlowItem::OnHandleEvent_Execute() {
  bool retCode = false;

  std::clock_t  timeA = 0,
                timeB = 0;

  switch (itemStatus) {
    case FlowItemStatus::unknown:

      switch (layoutOwnershipMode) {
        case LayoutOwnershipMode::make_copy:
          if (inputPorts.empty()) {
            p_logger->Warning(QString("Flow item '%1' has no input port but LayoutOwnershipMode was set to 'COPY'").arg(this->titleString));
            return false;
          }
          foreach (FlowItemPort *port, inputPorts) {
            if (port->IsConnected())
              continue;
            p_logger->Error(QString("Flow item '%1' has one of the inputs which is not connected").arg(this->titleString));
            return false;
          }
          if (!inputPorts[0]->GetLayout()) {
            p_logger->Error(QString("Flow item '%1' has nullptr layout at the input").arg(this->titleString));
            return false;
          }
          if (inputPorts[0]->GetLayout()->libraries.empty()) {
            p_logger->Error(QString("Flow item '%1' has empty layout at the input").arg(titleString));
            return false;
          }
          if (p_resultLayout) {
            delete p_resultLayout;
            p_resultLayout = nullptr;
          }
          p_resultLayout = new Layout;
          timeA = std::clock();
          LayoutLoader::GetInstance()->CopyLayout(inputPorts[0]->GetLayout(), p_resultLayout);
          timeB = std::clock();
          if (p_resultLayout->libraries.empty()) {
            p_logger->Error(QString("Copying layout for '%1' failed").arg(this->titleString));
            return false;
          }
          p_logger->Log(QString("Copying layout for '%1' took %2 ms").arg(this->titleString).arg(timeB - timeA));
          break;
        case LayoutOwnershipMode::make_link:
          if (inputPorts.empty()) {
            p_logger->Warning(QString("Flow item '%1' has no input port but LayoutOwnershipMode was set to 'LINK'").arg(this->titleString));
            return false;
          }
          foreach(FlowItemPort *port, inputPorts) {
            if (port->IsConnected())
              continue;
            p_logger->Error(QString("Flow item '%1' has one of the inputs which is not connected").arg(this->titleString));
            return false;
          }
          if (!inputPorts[0]->GetLayout()) {
            p_logger->Error(QString("Flow item '%1' has nullptr layout at the input").arg(this->titleString));
            return false;
          }
          if (inputPorts[0]->GetLayout()->libraries.empty()) {
            p_logger->Error(QString("Flow item '%1' has empty layout at the input").arg(titleString));
            return false;
          }
          p_resultLayout = inputPorts[0]->GetLayout();
          break;
        case LayoutOwnershipMode::make_new:
          if (p_resultLayout) {
            delete p_resultLayout;
            p_resultLayout = nullptr;
          }
          p_resultLayout = new Layout;
          break;
      }

      retCode = ExecuteEventHandler();
      if (retCode) {
        itemStatus = FlowItemStatus::completed;
        update();
        
        switch (layoutOwnershipMode) {
          case LayoutOwnershipMode::make_copy:
          case LayoutOwnershipMode::make_link:
          case LayoutOwnershipMode::make_new:
            if (!outputPorts.empty()) {
              outputPorts[0]->AssignLayout(p_resultLayout);

              if (!outputPorts[0]->connections.empty())
                for (int i = 0; i < outputPorts[0]->connections.size(); ++i)
                  outputPorts[0]->connections[i]->p_portB->AssignLayout(p_resultLayout);
            }
            break;
        }
      }
      return retCode;
    case FlowItemStatus::completed:
      return OpenResultsEventHandler();
    default:
      return false;
  }
}

bool FlowItem::OnHandleEvent_Reset() {
  bool retCode = ResetEventHandler();
  if (retCode) {
    itemStatus = FlowItemStatus::unknown;
    update();

    //TODO: Если модуль Export - уходить (у него нет outPorts[0])

    if(!outputPorts.empty())
      foreach(FlowItemConnection * connection, outputPorts[0]->connections) {
        if (connection->p_portB)
          if (connection->p_portB->GetOwner())
            if (connection->p_portB->GetOwner()->GetStatus() == FlowItemStatus::completed)
              connection->p_portB->GetOwner()->OnHandleEvent_Reset();
      }
  }
  return retCode;
}

void FlowItem::AddInputPort(PortDataType dataType) {
  FlowItemPort *p_port = new FlowItemPort(this, PortDirection::input, dataType);
  inputPorts.push_back(p_port);
  p_port->setPos(QPointF(p_port->pos().x(), p_port->pos().y() - ITEM_HEIGHT / 2 - PORT_RADIUS));

  int x = pos().x() - static_cast<qreal>(inputPorts.size() - 1) / 2.0 * static_cast<qreal>(PORT_MARGIN);

  foreach(QGraphicsItem *port, inputPorts) {
    port->setPos(QPointF(x, p_port->pos().y()));
    x += PORT_MARGIN;
  }
}

void FlowItem::AddOutputPort(PortDataType dataType) {
  if (!outputPorts.empty()) {
    p_logger->Warning("You can have only one output for an item. Operation ignored.");
    return;
  }
  FlowItemPort *p_port = new FlowItemPort(this, PortDirection::output, dataType);
  outputPorts.push_back(p_port);
  p_port->setPos(QPointF(p_port->pos().x(), p_port->pos().y() + ITEM_HEIGHT / 2 + PORT_RADIUS));  
}

void FlowItem::Disconnect() {
  foreach(FlowItemPort * port, inputPorts) {
    delete port;
    port = nullptr;
  }
  inputPorts.clear();
  foreach(FlowItemPort * port, outputPorts) {
    delete port;
    port = nullptr;
  }
  outputPorts.clear();
}

QVariant FlowItem::itemChange(GraphicsItemChange change, const QVariant &value) {
  if (!scene())
    return QGraphicsItem::itemChange(change, value);

  QPointF newPos;
  switch (change) {
    case ItemPositionChange:
      newPos = value.toPointF();
	    //https://www.walletfox.com/course/qgraphicsitemsnaptogrid.php
      //newPos.setX(round(newPos.x() / 20)*20);
      //newPos.setY(round(newPos.y() / 20)*20);
      //return newPos;
      break;
    /*
    case ItemSelectedChange:
      Logger::Get()->UpdateItemInfo(infoString);
      break;
    */
  }
  

  return QGraphicsItem::itemChange(change, value);
}

void FlowItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QPointF scenePoint = event->scenePos();
  QPointF itemPoint = QPointF(scenePoint.x() - pos().x(), scenePoint.y() - pos().y());

  if(event->button() == Qt::LeftButton) {
    
    // Check if "Execute" button was clicked
    if (itemPoint.x() >= pointBtnRun.x() && itemPoint.x() <= pointBtnRun.x() + 16 && itemPoint.y() >= pointBtnRun.y() && itemPoint.y() <= pointBtnRun.y() + 16) {
      OnHandleEvent_Execute();
      event->ignore();
      return;
    }
    // Check if "Properties" button was clicked
    if (itemPoint.x() >= pointBtnOptions.x() && itemPoint.x() <= pointBtnOptions.x() + 16 && itemPoint.y() >= pointBtnOptions.y() && itemPoint.y() <= pointBtnOptions.y() + 16) {
      if (ShowPropertesEventHandler())
        OnHandleEvent_Reset();
      event->ignore();
      return;
    }
  }
  QGraphicsItem::mousePressEvent(event);
}



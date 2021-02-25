#include "FlowItemPort.hpp"

#include <QPen>
#include <QPainter>

#include "FlowItem.hpp"
#include "FlowItemConnection.hpp"
#include "../Common.hpp"

FlowItemPort::FlowItemPort(FlowItem *parent, PortDirection dir, PortDataType data) : QGraphicsPathItem(parent), p_layout(nullptr), p_owner(parent), direction(dir), dataType(data) {
  QPainterPath p;
  p.addRoundedRect(-PORT_RADIUS, -PORT_RADIUS, 2.0 * PORT_RADIUS, 2.0 * PORT_RADIUS, 2, 2);
  setPath(p);

  setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

FlowItemPort::~FlowItemPort() {
  p_owner = nullptr;
  foreach(FlowItemConnection *p_connection, connections) {
    delete p_connection;
    p_connection = nullptr;
  }
  connections.clear();
}

bool FlowItemPort::IsConnectedWith(FlowItemPort *port) {
  foreach(FlowItemConnection *p_connection, connections)
    if (p_connection->p_portA == port || p_connection->p_portB == port)
      return true;

  return false;
}

bool FlowItemPort::IsConnected() {
  if (connections.empty())
    return false;
  else
    return true;
}

QVariant FlowItemPort::itemChange(GraphicsItemChange change, const QVariant &value) {
  if (change != ItemScenePositionHasChanged)
    return value;

  QPointF pt = pos();

  foreach(FlowItemConnection *p_connection, connections) {
    p_connection->UpdatePosFromPorts();
    p_connection->UpdatePath();
  }
  return value;
}

void FlowItemPort::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)

  switch (dataType) {
    case PortDataType::layout:
      painter->setPen(QPen(PIN_COLOR_LAYOUT_CONNECTED));
      if (connections.empty())
        painter->setBrush(PIN_COLOR_LAYOUT_EMPTY);
      else
        painter->setBrush(PIN_COLOR_LAYOUT_CONNECTED);
      break;
    case PortDataType::lef:
      painter->setPen(QPen(PIN_COLOR_LEF_CONNECTED));
      if (connections.empty())
        painter->setBrush(PIN_COLOR_LEF_EMPTY);
      else
        painter->setBrush(PIN_COLOR_LEF_CONNECTED);
      break;
    case PortDataType::def:
      painter->setPen(QPen(PIN_COLOR_DEF_CONNECTED));
      if (connections.empty())
        painter->setBrush(PIN_COLOR_DEF_EMPTY);
      else
        painter->setBrush(PIN_COLOR_DEF_CONNECTED);
      break;
    case PortDataType::verilog:
      painter->setPen(QPen(PIN_COLOR_VERILOG_CONNECTED));
      if (connections.empty())
        painter->setBrush(PIN_COLOR_VERILOG_EMPTY);
      else
        painter->setBrush(PIN_COLOR_VERILOG_CONNECTED);
      break;
  }

  painter->drawPath(path());

  if (direction == PortDirection::output) {
    painter->setPen(QPen(Qt::white));
    QFont f = painter->font();
    f.setPointSize(6);
    painter->setFont(f);
    painter->drawText(QPoint(-2, 3), QString("%1").arg(connections.size()));
  }
}



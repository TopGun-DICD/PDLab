#include "FlowItemPort.hpp"

#include <QPen>
#include <QPainter>

#include "FlowItem.hpp"
#include "FlowItemConnection.hpp"

FlowItemPort::FlowItemPort(FlowItem *parent, PortDirection dir) : QGraphicsPathItem(parent), p_layout(nullptr), p_owner(parent), direction(dir) {
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

  painter->setPen(QPen(Qt::darkRed));
  if (connections.empty())
    painter->setBrush(Qt::red);
  else
    painter->setBrush(Qt::darkRed);

  painter->drawPath(path());

  if (direction == PortDirection::output) {
    painter->setPen(QPen(Qt::white));
    QFont f = painter->font();
    f.setPointSize(6);
    painter->setFont(f);
    painter->drawText(QPoint(-2, 3), QString("%1").arg(connections.size()));
  }
}


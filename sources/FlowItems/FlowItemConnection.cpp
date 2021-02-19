#include "FlowItemConnection.hpp"

#include <QPen>

#include "FlowItem.hpp"

FlowItemConnection::FlowItemConnection() : QGraphicsPathItem(nullptr), p_portA(nullptr), p_portB(nullptr) {
  setPen(QPen(Qt::black, 2));
  setBrush(Qt::NoBrush);
  setZValue(-1);
}

FlowItemConnection::~FlowItemConnection() {
  if (p_portA)
    p_portA->connections.remove(p_portA->connections.indexOf(this));
  if (p_portB) {
    if(p_portB->GetOwner())
      if (p_portB->GetOwner()->GetStatus() == FlowItemStatus::completed) {
        p_portB->AssignLayout(nullptr);
        p_portB->GetOwner()->OnHandleEvent_Reset(); 
      }
    p_portB->connections.remove(p_portB->connections.indexOf(this));
  }
}

void FlowItemConnection::AssignPortA(FlowItemPort *port) {
  p_portA = port;
  p_portA->connections.append(this);
}

void FlowItemConnection::AssignPortB(FlowItemPort *port) {
  FlowItemPort *temp = nullptr;
  switch (port->direction) {
    case PortDirection::input:
      p_portB = port;
      p_portB->connections.append(this);
      break;
    case PortDirection::output:
      p_portB = p_portA;
      p_portA = port;
      p_portA->connections.append(this);
  }
  
}

void FlowItemConnection::SetPositionA(const QPointF &pt) {
  posA = pt;
}

void FlowItemConnection::SetPositionB(const QPointF &pt) {
  posB = pt;
}

void FlowItemConnection::UpdatePath() {
  QPainterPath p;

  p.moveTo(posA);

  qreal dx = posB.x() - posA.x();
  qreal dy = posB.y() - posA.y();

  QPointF ctr1(posA.x() + dx * 0.25, posA.y() + dy * 0.1);
  QPointF ctr2(posA.x() + dx * 0.75, posA.y() + dy * 0.9);

  p.cubicTo(ctr1, ctr2, posB);

  setPath(p);
}

void FlowItemConnection::UpdatePosFromPorts() {
  posA = p_portA->scenePos();
  posB = p_portB->scenePos();
}


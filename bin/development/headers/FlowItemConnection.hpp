#pragma once

#include <QGraphicsPathItem>

class FlowItemPort;

class FlowItemConnection : public QGraphicsPathItem {
public:
  enum { Type = QGraphicsItem::UserType + 3 };
public:
  FlowItemPort *p_portA,
               *p_portB;
  QPointF       posA, 
                posB;
public:
  FlowItemConnection();
 ~FlowItemConnection();
public:
  void AssignPortA(FlowItemPort *port);
  void AssignPortB(FlowItemPort *port);
  void SetPositionA(const QPointF &pt);
  void SetPositionB(const QPointF &pt);
  void UpdatePath();
  void UpdatePosFromPorts();
  int type() const { return /*Type*/QGraphicsItem::UserType + 3; }
  FlowItemPort *GetPortA() { return p_portA; }
  FlowItemPort *GetPortB() { return p_portB; }
};

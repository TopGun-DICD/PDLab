#pragma once

#include <QGraphicsPathItem>

#define PORT_RADIUS 5
#define PORT_MARGIN 3*PORT_RADIUS

enum PortDirection {
  pd_undefnied = 0,
  pd_input,
  pd_output,
};

class FlowItem;
class FlowItemConnection;

class FlowItemPort : public QGraphicsPathItem {
public:
  enum { Type = QGraphicsItem::UserType + 2 };
public:
  QVector<FlowItemConnection *> connections;
public:
  FlowItem       *p_owner;
  PortDirection   direction;
public:
  FlowItemPort(FlowItem *parent, PortDirection dir);
 ~FlowItemPort();
public:
  bool IsConnectedWith(FlowItemPort *port);
  QVariant itemChange(GraphicsItemChange change, const QVariant &value);
  int type() const { return /*Type*/QGraphicsItem::UserType + 2; }
  FlowItem *GetOwner() { return p_owner; }
private:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#pragma once

#include <QGraphicsPathItem>

#define PORT_RADIUS 5.0
#define PORT_MARGIN 3.0 * PORT_RADIUS

#include <Layout.hpp>

#include "../Types.hpp"

class FlowItem;
class FlowItemConnection;

class FlowItemPort : public QGraphicsPathItem {
  Layout         *p_layout;
public:
  enum { Type = QGraphicsItem::UserType + 2 };
public:
  QVector<FlowItemConnection *> connections;
public:
  FlowItem       *p_owner;
  PortDirection   direction;
  PortDataType    dataType;
public:
  FlowItemPort(FlowItem *parent, PortDirection dir, PortDataType data = PortDataType::undefined);
 ~FlowItemPort();
public:
  bool IsConnectedWith(FlowItemPort *port);
  bool IsConnected();
  QVariant itemChange(GraphicsItemChange change, const QVariant &value);
  int type() const { return QGraphicsItem::UserType + 2; }
  FlowItem *GetOwner() { return p_owner; }
  // Layout operations
  void AssignLayout(Layout *layout) { p_layout = layout; }
  Layout *GetLayout() { return p_layout; }
private:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

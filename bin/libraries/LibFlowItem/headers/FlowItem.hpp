#pragma once

#include <QGraphicsPathItem>

#include "Types.hpp"
#include "FlowItemPort.hpp"
#include "Logger.hpp"

class FlowItem : public QGraphicsPathItem {
  FlowItemType              itemType;
  QPainterPath              pathShape,
                            pathHeader;
protected:
  FlowItemStatus            itemStatus;
  BasicLogger              *p_logger;
  QString                   titleString,
                            topString,
                            bottomString;
  QColor                    titleBgColor;
public:
  QVector <FlowItemPort *>  inputPorts,
                            outputPorts;
public:
  FlowItem(FlowItemType type, QString title, BasicLogger *logger);
  virtual ~FlowItem();
public:
  virtual bool OnHandleEvent_Drop() = 0;
  virtual bool OnHandleEvent_Execute() = 0;
  virtual bool OnHandleEvent_Reset() = 0;
  virtual bool OnHandleEvent_Properties() = 0;
  virtual QString GetItemTypeAsString() = 0;
  virtual QString GetInfoString() = 0;
public:
  void addInputPort();
  void addOutputPort();
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

  int type() const { return QGraphicsItem::UserType + 1; }
  FlowItemType FlowType() { return itemType; }
  FlowItemStatus GetStatus() { return itemStatus; }
protected:
  QVariant itemChange(GraphicsItemChange change, const QVariant &value);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

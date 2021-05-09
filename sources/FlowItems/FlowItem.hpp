#pragma once

#include <QGraphicsPathItem>

#include "../Types.hpp"
#include "FlowItemPort.hpp"
#include "../Logger.hpp"

#include <Layout.hpp>

class FlowItem : public QGraphicsPathItem {
  FlowItemType              itemType;
  QPainterPath              pathShape,
                            pathHeader;
  LayoutOwnershipMode       layoutOwnershipMode;
protected:
  FlowItemStatus            itemStatus;
  BasicLogger              *p_logger;
  QString                   titleString,
                            topString,
                            bottomString;
  QColor                    titleBgColor;
  Layout                   *p_resultLayout;
public:
  enum { Type = QGraphicsItem::UserType + 1 };
public:
  QVector <FlowItemPort *>  inputPorts,
                            outputPorts;
public:
  FlowItem(FlowItemType type, QString title, BasicLogger *logger, LayoutOwnershipMode mode);
  virtual ~FlowItem();
public:
  virtual bool DropEventHandler() = 0;
  virtual bool ExecuteEventHandler() = 0;
  virtual bool OpenResultsEventHandler() = 0;
  virtual bool ResetEventHandler() = 0;
  virtual bool ShowPropertesEventHandler() = 0;

  virtual QString GetInfoString() = 0;
  virtual QString GetItemTypeAsString() = 0;
public:
  void AddInputPort(PortDataType dataType);
  void AddOutputPort(PortDataType dataType);
  void Disconnect();

  bool OnHandleEvent_Execute();
  bool OnHandleEvent_Reset();

  int type() const { return QGraphicsItem::UserType + 1; }
  FlowItemType GetItemType() { return itemType; }
  FlowItemStatus GetStatus() { return itemStatus; }
private:
  QVariant itemChange(GraphicsItemChange change, const QVariant &value);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

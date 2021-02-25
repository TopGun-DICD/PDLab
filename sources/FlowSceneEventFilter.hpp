#pragma once

#include <QGraphicsView>

#include "Logger.hpp"

class FlowItemConnection;

class FlowSceneEventFilter : public QObject {
  Q_OBJECT
private:
  BasicLogger        *p_logger;
  QGraphicsView      *p_parentView;
  QGraphicsScene     *p_scene;
  FlowItemConnection *p_connection;
  double              zIndex;
public:
  FlowSceneEventFilter(QWidget *parent, BasicLogger *logger);
public:
  void AssignGraphicsScene(QGraphicsScene *scene);
private:
  QGraphicsItem *GetItemAtXY(const QPointF&);
  bool eventFilter(QObject *, QEvent *);

  bool OnMousePress(QObject *object, QGraphicsSceneMouseEvent *event);
  bool OnMouseMove(QObject *object, QGraphicsSceneMouseEvent *event);
  bool OnMouseRelease(QObject *object, QGraphicsSceneMouseEvent *event);
  bool OnKeyPress(QObject *object, QKeyEvent *event);
};

#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>

#include "FlowSceneEventFilter.hpp"
#include "Logger.hpp"
#include "UserFlowItemsManager.hpp"

class FlowScene : public QGraphicsScene {
  Q_OBJECT
private:
  Logger *p_logger;
public:
  FlowScene(QWidget* parent, Logger *logger);
private:
  void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
private slots:
  void OnSelectionChanged();
};

class FlowWidget : public QGraphicsView {
  Q_OBJECT
private:
  Logger *p_logger;
  UserFlowItemsManager *p_manager;
public:
  FlowScene *p_scene;
  FlowSceneEventFilter *p_eventFilter;
public:
  FlowWidget(QWidget *parent, Logger *logger, UserFlowItemsManager *manager);
 ~FlowWidget();
private:
  void dragEnterEvent(QDragEnterEvent* event);
  void dropEvent(QDropEvent* event);
};

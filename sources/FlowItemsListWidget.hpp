#pragma once

#include <QPushButton>
#include <QTreeWidget>
#include <QListWidget>

#include "Types.hpp"
#include "UserFlowItemsManager.hpp"

class CategoryButton : public QPushButton {
  Q_OBJECT
public:
  CategoryButton(const QString &text, QWidget *parent, QTreeWidgetItem *item);
private slots:
  void ButtonPressed();
private:
  QTreeWidgetItem *p_item;
};

class CustomListItem : public QWidget {
  QPixmap       icon;
  FlowItemType  type;
  int           uniqueID;
public:
  CustomListItem(FlowItemType it, const QPixmap &pixmap, const QString &text, int id = 0);
public:
  QPixmap& GetPixmap();
  FlowItemType GetType();
  int GetUniueID();
};

class FlowItemsListWidget : public QTreeWidget {
public:
  FlowItemsListWidget(QWidget *parent);
private:
  void FillSection_LayoutIO(QPainter &painter, QPixmap &px, QFontMetrics &fm);
  void FillSection_LayerOperations(QPainter &painter, QPixmap &px, QFontMetrics &fm);
  void FillSection_LayoutOperations(QPainter &painter, QPixmap &px, QFontMetrics &fm);
};

class ItemsListWidget : public QListWidget {
public:
  ItemsListWidget(QWidget* parent = nullptr);
public:
  void startDrag(Qt::DropActions supportedActions);
};

class UserFlowItemsListWidget : public QTreeWidget {
public:
  UserFlowItemsListWidget(QWidget *parent, UserFlowItemsManager *manager);
};

class UserItemsListWidget : public QListWidget {
public:
  UserItemsListWidget(QWidget* parent = nullptr);
public:
  void startDrag(Qt::DropActions supportedActions);
};
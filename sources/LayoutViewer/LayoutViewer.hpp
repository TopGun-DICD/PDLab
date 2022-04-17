#pragma once

#include "LayoutView.hpp"

class LayoutViewer : public QWidget {
  Q_OBJECT
private:
  LayoutView* p_view;
public:
  LayoutViewer(QWidget* parent);
public:
  void AssignLayout(Layout* layout);
  void UpdateLayout();
};
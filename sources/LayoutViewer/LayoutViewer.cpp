#include "LayoutViewer.hpp"

#include <QToolBar>
#include <QLayout>

LayoutViewer::LayoutViewer(QWidget* parent) : QWidget(parent) {
  QToolBar *toolbar = new QToolBar();
  toolbar->addAction("foo");
  toolbar->addAction("bar");
  //toolbar->setStyleSheet("QToolBar {border: 1px solid blue }");

  p_view = new LayoutView(this);

  QVBoxLayout *layout = new QVBoxLayout();
  layout->setMenuBar(toolbar);
  layout->addWidget(p_view);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
}

void LayoutViewer::AssignLayout(Layout* layout) {
  if(p_view)
    p_view->AssignLayout(layout);
}

void LayoutViewer::UpdateLayout() {
  p_view->update();
  p_view->repaint();
}

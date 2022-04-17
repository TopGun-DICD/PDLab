#include "LayoutView.hpp"

#include <QPainter>
#include <QPaintEvent>

LayoutView::LayoutView(QWidget* parent) : QOpenGLWidget(parent) {
}

void LayoutView::paintEvent(QPaintEvent* event) {
  QPainter painter;
  painter.begin(this);

  painter.fillRect(event->rect(), QBrush(QColor(0, 0, 0)));

  if (!p_layout) {
    painter.end();
    return;
  }

  PrepareCoefficients();

  painter.setPen(Qt::white);

  int x1 = xPlus + (p_layout->libraries[0]->min.x - p_layout->libraries[0]->min.x) * k + 5;
  int y1 = (p_layout->libraries[0]->min.y - p_layout->libraries[0]->min.y) * k + 5;
  int x2 = (p_layout->libraries[0]->max.x - p_layout->libraries[0]->min.x) * k;
  int y2 = (p_layout->libraries[0]->max.y - p_layout->libraries[0]->min.y) * k;
  painter.drawRect(x1, y1, x2, y2);

  for (size_t i = 0; i < p_layout->libraries[0]->elements.size(); ++i) {
    if (p_layout->libraries[0]->elements[i]->nested)
      continue;

    DrawLayoutElement(painter, p_layout->libraries[0]->elements[i]);
  }

  painter.end();
}

void LayoutView::AssignLayout(Layout* layout) {
  p_layout = layout;
}

void LayoutView::PrepareCoefficients() {
  QRect rectLayout(QPoint(p_layout->libraries[0]->min.x, p_layout->libraries[0]->min.y), QPoint(p_layout->libraries[0]->max.x, p_layout->libraries[0]->max.y));
  QRect rectWidget = this->rect();

  H = rectWidget.height();

  kX = (double)(rectWidget.width() - 10) / (double)rectLayout.width();
  kY = (double)(rectWidget.height() - 10) / (double)rectLayout.height();

  k = (kX < kY) ? kX : kY;

  xPlus = rectWidget.width() / 2 - (rectLayout.width() * k) / 2;
  yPlus = rectWidget.height() / 2 - (rectLayout.height() * k) / 2;

}

void LayoutView::DrawLayoutElement(QPainter& painter, Element* element, Coord offset) {
  int x1 = 0,
    y1 = 0,
    x2 = 0,
    y2 = 0;

  Path       *p_path = nullptr;
  Reference  *p_ref = nullptr;

  for (size_t j = 0; j < element->geometries.size(); ++j) {
    Geometry* p_geometry = element->geometries[j];

    switch (p_geometry->type) {
    case GeometryType::rectangle:
      if (p_geometry->coords.size() == 5) {
        x1 = xPlus + (offset.x + p_geometry->coords[0].x - p_layout->libraries[0]->min.x) * k + 5;
        y1 = H - (offset.y + p_geometry->coords[0].y - p_layout->libraries[0]->min.y) * k - 5;
        x2 = xPlus + (offset.x + p_geometry->coords[2].x - p_layout->libraries[0]->min.x) * k + 5;
        y2 = H - (offset.y + p_geometry->coords[2].y - p_layout->libraries[0]->min.y) * k - 5;
        painter.drawRect(x1, y1, x2 - x1, y2 - y1);
      }
      break;
    case GeometryType::polygon:
      if (p_geometry->coords.size() == 5) {
        x1 = xPlus + (offset.x + p_geometry->coords[0].x - p_layout->libraries[0]->min.x) * k + 5;
        y1 = H - (offset.y + p_geometry->coords[0].y - p_layout->libraries[0]->min.y) * k - 5;
        x2 = xPlus + (offset.x + p_geometry->coords[2].x - p_layout->libraries[0]->min.x) * k + 5;
        y2 = H - (offset.y + p_geometry->coords[2].y - p_layout->libraries[0]->min.y) * k - 5;
        painter.drawRect(x1, y1, x2 - x1, y2 - y1);
      }
      else {
        for (size_t m = 1; m < p_geometry->coords.size(); ++m) {
          x1 = xPlus + (offset.x + p_geometry->coords[m - 1].x - p_layout->libraries[0]->min.x) * k + 5;
          y1 = H - (offset.y + p_geometry->coords[m - 1].y - p_layout->libraries[0]->min.y) * k - 5;
          x2 = xPlus + (offset.x + p_geometry->coords[m].x - p_layout->libraries[0]->min.x) * k + 5;
          y2 = H - (offset.y + p_geometry->coords[m].y - p_layout->libraries[0]->min.y) * k - 5;
          painter.drawLine(x1, y1, x2, y2);
        }
      }
      break;
    case GeometryType::path:
      p_path = static_cast<Path*>(p_geometry);
      for (size_t m = 1; m < p_path->coords.size(); ++m) {
        x1 = xPlus + (offset.x + p_path->coords[m - 1].x - p_layout->libraries[0]->min.x) * k + 5;
        y1 = H - (offset.y + p_path->coords[m - 1].y - p_layout->libraries[0]->min.y) * k - 5;
        x2 = xPlus + (offset.x + p_path->coords[m].x - p_layout->libraries[0]->min.x) * k + 5;
        y2 = H - (offset.y + p_path->coords[m].y - p_layout->libraries[0]->min.y) * k - 5;
        painter.drawLine(x1, y1, x2, y2);
      }
      break;
    case GeometryType::reference:
      p_ref = static_cast<Reference*>(p_geometry);
      DrawLayoutElement(painter, p_ref->referenceTo, p_ref->coords[0]);
      break;
    }
  }
}
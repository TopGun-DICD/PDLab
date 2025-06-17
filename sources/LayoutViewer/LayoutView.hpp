#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <Layout.hpp>

const Coord zeroOffset = { 0, 0 };

class LayoutView : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
private:
  Layout *p_layout;
  int     H, W, xPlus, yPlus;
  double  k, kX, kY;
public:
  LayoutView(QWidget *parent);
public:
  void AssignLayout(Layout *layout);
protected:
  void paintEvent(QPaintEvent *event);
private:
  void PrepareCoefficients();
  void DrawLayoutElement(QPainter &painter, Element *element, Coord offset = zeroOffset);
};
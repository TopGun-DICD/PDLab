#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <Layout.hpp>

const Coord zeroOffset = { 0, 0 };

class LayoutViewer : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
private:
  Layout *p_layout;
  int     H, W, xPlus, yPlus;
  double  k, kX, kY;
public:
  LayoutViewer(QWidget *parent);
public:
  void AssignLayout(Layout *layout);
protected:
  void paintEvent(QPaintEvent *event);
  //void initializeGL();
  //void resizeEvent(QResizeEvent *event);
  //void paintGL();
private:
  void PrepareCoefficients();
  void DrawLayoutElement(QPainter &painter, Element *element, Coord offset = zeroOffset);
};

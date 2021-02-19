#pragma once

#include <QDialog>
#include <QTableWidget>

#include <Layout.hpp>

class Dlg_Map : public QDialog {
  Q_OBJECT
private:
  QTableWidget *p_layers;
  Layout       *p_layout;
public:
  Dlg_Map(QWidget *parent, Layout *layout);
public:
  virtual void accept() override final;
};

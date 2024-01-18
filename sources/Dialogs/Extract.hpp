#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QListWidget>

struct LayerInfo {
  QString name,
          alias;
  int16_t id,
          dataType;
  bool    selected;
};

struct LayersInfo {
  QVector<int>        selected;
  QVector<LayerInfo>  layers;
  QString             mask;
};

class Dlg_Extract : public QDialog {
  Q_OBJECT
private:
  QLineEdit    *p_maskEdit;
  QListWidget  *p_selectedLayers;
  LayersInfo   *p_layersInfo;
public:
  Dlg_Extract(QWidget *parent, LayersInfo *li);
public:
  virtual void accept() override final;
};

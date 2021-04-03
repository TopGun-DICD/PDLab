#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>

class Dlg_Export : public QDialog {
  Q_OBJECT
public:
  QLineEdit  *p_fileLayout,
             *p_excludeLayers;
  QCheckBox  *p_cbPathsAsPolygons,
             *p_cbRectsAsPolygons,
             *p_cbFlattenLayout;
public:
  Dlg_Export(QWidget *parent);
public:
  virtual void accept() override final;
private slots:
  void OnSaveLayoutClick();
  //void OnOpenMappingClick();
};

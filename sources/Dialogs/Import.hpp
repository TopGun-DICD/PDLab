#pragma once

#include <QDialog>
#include <QLineEdit>

class Dlg_Import : public QDialog {
  Q_OBJECT
public:
  QLineEdit  *p_fileLayout,
             *p_fileMapping,
             *p_maskLayers;
public:
  Dlg_Import(QWidget *parent);
public:
  virtual void accept() override final;
private slots:
  void OnOpenLayoutClick();
  void OnOpenMappingClick();
};

#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>

class Dlg_Import : public QDialog {
  Q_OBJECT
public:
  QLineEdit  *p_fileLayout,
             *p_fileMapping,
             *p_layers2Read,
             *p_layers2Ignore,
             *p_cells2Read;
  QCheckBox  *p_cbConvertBoundaries,
             *p_cbIgnoreZeroWidthPath;
public:
  Dlg_Import(QWidget *parent);
public:
  virtual void accept() override final;
private slots:
  void OnOpenLayoutClick();
  void OnOpenMappingClick();
};

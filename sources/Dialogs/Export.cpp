#include "Export.hpp"

#include <QVBoxLayout>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>

Dlg_Export::Dlg_Export(QWidget *parent) : QDialog(parent) {
  setWindowTitle(tr("Export Layout"));

  QVBoxLayout *p_globalLayout = new QVBoxLayout(this);

  QVBoxLayout *p_vLayout = nullptr;

  QHBoxLayout *p_hLayout = new QHBoxLayout(this);
  
  p_vLayout = new QVBoxLayout(this);
  p_vLayout->addWidget(new QLabel("Options:", this));
  p_cbPathsAsPolygons = new QCheckBox("Paths as polygons", this);
  p_cbPathsAsPolygons->setEnabled(false);
  p_vLayout->addWidget(p_cbPathsAsPolygons);
  p_cbRectsAsPolygons = new QCheckBox("Rectangle polygons as boxes", this);
  p_cbRectsAsPolygons->setEnabled(false);
  p_vLayout->addWidget(p_cbRectsAsPolygons);
  p_cbFlattenLayout = new QCheckBox("Flatten layout", this);
  p_cbFlattenLayout->setEnabled(false);
  p_vLayout->addWidget(p_cbFlattenLayout);

  p_hLayout->addLayout(p_vLayout);

  p_vLayout = new QVBoxLayout(this);
  
  p_vLayout->addWidget(new QLabel("Layout:", this));
  QHBoxLayout *p_hL1 = new QHBoxLayout;
  p_fileLayout = new QLineEdit(this);
  p_fileLayout->setMinimumWidth(400);
  p_hL1->addWidget(p_fileLayout, 1);
  QPushButton *p_btnOpenLayout = new QPushButton("...", this);
  p_btnOpenLayout->adjustSize();
  connect(p_btnOpenLayout, SIGNAL(clicked()), this, SLOT(OnSaveLayoutClick()));
  p_hL1->addWidget(p_btnOpenLayout, 0, Qt::AlignRight);
  p_vLayout->addLayout(p_hL1);

  p_vLayout->addWidget(new QLabel("Exclude layers:", this));
  QHBoxLayout *p_hL2 = new QHBoxLayout;
  p_excludeLayers = new QLineEdit(this);
  p_excludeLayers->setMinimumWidth(300);
  p_hL2->addWidget(p_excludeLayers, 1);
  p_vLayout->addLayout(p_hL2);

  p_hLayout->addLayout(p_vLayout);

  p_globalLayout->addLayout(p_hLayout);
  
  QDialogButtonBox *p_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  connect(p_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(p_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  p_globalLayout->addWidget(p_buttonBox);

  setLayout(p_globalLayout);
}

void Dlg_Export::accept() {
  if (p_fileLayout->text().isEmpty())
    return;
  //QFileInfo fi;
  //fi.setFile(p_fileLayout->text());
  //if (!fi.exists() || !fi.isFile())
  //  return;

  /*
  if (!p_fileMapping->text().isEmpty()) {
    fi.setFile(p_fileMapping->text());
    if (fi.suffix() != "tf")
      return;
    if (!fi.exists() || !fi.isFile())
      return;
  }
  */
  QDialog::accept();
}

void Dlg_Export::OnSaveLayoutClick() {
  QString fileName = QFileDialog::getSaveFileName(nullptr, QString("Save Layout As"), ".", QString("GDSII Files (*.gds *.gdsii);;MSK Files(*.msk)"));
  if (fileName.isEmpty())
    return;
  p_fileLayout->setText(fileName);
}

/*
void Dlg_Export::OnOpenMappingClick() {
  QString fileName = QFileDialog::getOpenFileName(nullptr, QString("Open Tech File"), ".", QString("Cadence Tech Files (*.tf)"));
  if (fileName.isEmpty())
    return;
  p_fileMapping->setText(fileName);
}
*/

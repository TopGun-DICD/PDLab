#include "Import.hpp"

#include <QVBoxLayout>
#include <QBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>

Dlg_Import::Dlg_Import(QWidget *parent) : QDialog(parent) {
  setWindowTitle(tr("Import Layout"));

  QVBoxLayout *p_vLayout = new QVBoxLayout(this);
  
  QHBoxLayout *p_hLayout = nullptr;

  p_vLayout->addWidget(new QLabel("Layout:", this));
  p_hLayout = new QHBoxLayout;
  p_fileLayout = new QLineEdit(this);
  p_fileLayout->setMinimumWidth(400);
  p_hLayout->addWidget(p_fileLayout, 1);
  QPushButton *p_btnOpenLayout = new QPushButton("...", this);
  p_btnOpenLayout->adjustSize();
  connect(p_btnOpenLayout, SIGNAL(clicked()), this, SLOT(OnOpenLayoutClick()));
  p_hLayout->addWidget(p_btnOpenLayout, 0, Qt::AlignRight);
  p_vLayout->addLayout(p_hLayout);

  p_vLayout->addWidget(new QLabel("Mapping File:", this));
  p_hLayout = new QHBoxLayout;
  p_fileMapping = new QLineEdit(this);
  p_fileMapping->setMinimumWidth(300);
  p_hLayout->addWidget(p_fileMapping, 1);
  QPushButton *p_btnOpenMapping = new QPushButton("...", this);
  p_btnOpenMapping->adjustSize();
  connect(p_btnOpenMapping, SIGNAL(clicked()), this, SLOT(OnOpenMappingClick()));
  p_hLayout->addWidget(p_btnOpenMapping, 0, Qt::AlignRight);
  p_vLayout->addLayout(p_hLayout);

  QGroupBox* groupBox = new QGroupBox(tr("Read options"));
  QFormLayout* layout = new QFormLayout;
  p_layers2Read = new QLineEdit(this);
  p_layers2Read->setText("*");
  layout->addRow(new QLabel("Layers to read", this), p_layers2Read);
  p_layers2Ignore = new QLineEdit(this);
  p_layers2Ignore->setText("none");
  layout->addRow(new QLabel("Layers to ignore", this), p_layers2Ignore);
  p_cbConvertBoundaries = new QCheckBox("Convert boundaries (polygons) to boxes (rectangles) if possible", this);
  p_cbConvertBoundaries->setEnabled(false);
  layout->addRow(nullptr, p_cbConvertBoundaries);
  p_cbIgnoreZeroWidthPath = new QCheckBox("Ignore zero-width paths", this);
  p_cbIgnoreZeroWidthPath->setEnabled(false);
  layout->addRow(nullptr, p_cbIgnoreZeroWidthPath);
  groupBox->setLayout(layout);
  p_vLayout->addWidget(groupBox);

  QDialogButtonBox *p_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  connect(p_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(p_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  p_vLayout->addWidget(p_buttonBox);
}

void Dlg_Import::accept() {
  if (p_fileLayout->text().isEmpty())
    return;
  QFileInfo fi;
  fi.setFile(p_fileLayout->text());
  if (!fi.exists() || !fi.isFile())
    return;

  if (!p_fileMapping->text().isEmpty()) {
    fi.setFile(p_fileMapping->text());
    if (fi.suffix() != "tf")
      return;
    if (!fi.exists() || !fi.isFile())
      return;
  }
  QDialog::accept();
}

void Dlg_Import::OnOpenLayoutClick() {
  QString fileName = QFileDialog::getOpenFileName(nullptr, QString("Open Layout File"), "./examples/", QString("GDSII Binary Files (*.gds *.gdsii);;Microwind Files(*.msk)"));
  if (fileName.isEmpty())
    return;
  p_fileLayout->setText(fileName);
}

void Dlg_Import::OnOpenMappingClick() {
  QString fileName = QFileDialog::getOpenFileName(nullptr, QString("Open Tech File"), "./examples/", QString("Cadence Tech Files (*.tf)"));
  if (fileName.isEmpty())
    return;
  p_fileMapping->setText(fileName);
}
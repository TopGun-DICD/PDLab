#include "Import.hpp"

#include <QVBoxLayout>
#include <QBoxLayout>
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
  p_fileLayout->setMinimumWidth(300);
  p_hLayout->addWidget(p_fileLayout, 1);
  QPushButton *p_btnOpenLayout = new QPushButton("...", this);
  //p_btnOpenLayout->adjustSize();
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
  
  QDialogButtonBox *p_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  connect(p_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(p_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  p_vLayout->addWidget(p_buttonBox);
}

void Dlg_Import::accept() {
  if (p_fileLayout->text().isEmpty())
    return;
  QFileInfo fi(p_fileLayout->text());
  if (!fi.exists() || !fi.isFile())
    return;
  QDialog::accept();
}

void Dlg_Import::OnOpenLayoutClick() {
  QString fileName = QFileDialog::getOpenFileName(nullptr, QString("Open Layout File"), ".", QString("GDSII Files (*.gds *.gdsii);;MSK Files(*.msk)"));
  if (fileName.isEmpty())
    return;
  p_fileLayout->setText(fileName);
}

void Dlg_Import::OnOpenMappingClick() {
  QString fileName = QFileDialog::getOpenFileName(nullptr, QString("Open Layout Mapping File"), ".", QString("MAP Files (*.map)"));
  if (fileName.isEmpty())
    return;
  p_fileMapping->setText(fileName);
}
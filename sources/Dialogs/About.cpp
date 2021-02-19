#include "About.hpp"

#include <QVBoxLayout>
#include <QImage>
#include <QLabel>
#include <QDialogButtonBox>

#include "../Common.hpp"

Dlg_About::Dlg_About(QWidget *parent) : QDialog(parent) {
  setWindowTitle(tr("About PDLab"));

  QVBoxLayout *p_layout = new QVBoxLayout(this);
  
  QImage pic(":/dialogs/logo.png");
  QLabel *p_logoLabel = new QLabel(this);
  p_logoLabel->setPixmap(QPixmap::fromImage(pic));
  p_layout->addWidget(p_logoLabel);

  QLabel *p_labelVersion = new QLabel(QString("PDLab version %1").arg(VERSION_STRING));
  p_layout->addWidget(p_labelVersion);
  QLabel *p_buildTime = new QLabel(QString("Built on %1 %2").arg(__DATE__).arg(__TIME__));
  p_layout->addWidget(p_buildTime);

  //p_layout->addWidget(button3);
  //p_layout->addWidget(button4);
  
  
  QDialogButtonBox *p_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok); 
  connect(p_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  p_layout->addWidget(p_buttonBox);
}
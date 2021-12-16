#include "About.hpp"

#include <QVBoxLayout>
#include <QImage>
#include <QLabel>
#include <QDialogButtonBox>
#include <QFont>
#include <QTextEdit>

#include "../Config.hpp"

const QString htmlText(
  "<b>Physical Design Laborator (PDLab)</b> is an open-source software written in C++ and built with Qt5.14.1.<br/><br/>"
  "Persons who are involved in development:"
  "<ul>"
  "<li><b>Dmitry Bulakh</b>     - developer <br/> National Research University of Electronic Technology, dept. of IC design</li>"
  "<li><b>Andrey Korshunov</b>  - consultant<br/> National Research University of Electronic Technology, dept. of IC design</li>"
  "<li><b>Sergey Morozov</b>    - consultant<br/> Zelenograd Innovation-Technology Center, dept. of photomasks design and manufacturing</li>"
  "<li><b>Sergei Ilin</b>       - consultant<br/> Molecular Electronics Research Institute, lab. of libraries characterization</li>"
  "</ul>"
);

Dlg_About::Dlg_About(QWidget *parent) : QDialog(parent) {
  setWindowTitle(tr("About PDLab"));

  QVBoxLayout *p_globalLayout = new QVBoxLayout(this);
  p_globalLayout->setMargin(0);
  
  QHBoxLayout *p_logoLayout = new QHBoxLayout(this);
  p_logoLayout->setAlignment(Qt::AlignBottom);

  QImage pic(":/dialogs/logo.png");
  QLabel *p_logoLabel = new QLabel(this);
  p_logoLabel->setPixmap(QPixmap::fromImage(pic));
  p_logoLayout->addWidget(p_logoLabel);
  
  QVBoxLayout *p_textLayout = new QVBoxLayout(this);
  p_textLayout->setMargin(10);

  QLabel *p_labelName = new QLabel("Physical Design Laboratory");
  QFont f(p_labelName->font().family());
  f.setPointSize(24);
  f.setWeight(QFont::Bold);
  p_labelName->setFont(f);
  p_textLayout->addWidget(p_labelName);
  QLabel *p_labelVersion = new QLabel(QString("PDLab version %1").arg(Config::Get()->versionString));
  p_textLayout->addWidget(p_labelVersion);
  QLabel *p_buildTime = new QLabel(QString("Built on %1 %2").arg(__DATE__).arg(__TIME__));
  p_textLayout->addWidget(p_buildTime);
  QTextEdit *p_infoText = new QTextEdit(this);
  p_textLayout->addWidget(p_infoText);
  p_infoText->setFixedHeight(235);
  //p_infoText->setEd(false);
  p_infoText->setReadOnly(true);
  p_infoText->setHtml(htmlText);

  p_logoLayout->addLayout(p_textLayout);
  p_globalLayout->addLayout(p_logoLayout);
  
  QVBoxLayout *p_buttonLayout = new QVBoxLayout(this);
  p_buttonLayout->setMargin(10);
  QDialogButtonBox *p_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok); 
  connect(p_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

  p_buttonLayout->addWidget(p_buttonBox);
  p_globalLayout->addLayout(p_buttonLayout);
}
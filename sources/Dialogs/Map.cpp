#include "Map.hpp"

#include <QVBoxLayout>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QStringList>
#include <QHeaderView>

Dlg_Map::Dlg_Map(QWidget *parent, Layout *layout) : QDialog(parent), p_layers(nullptr), p_layout(layout) {
  setWindowTitle(tr("Map Layers"));
  resize(QSize(300, 400));

  QVBoxLayout *p_vLayout = new QVBoxLayout(this);
  
  p_layers = new QTableWidget(this);
  p_layers->resize(280, 400);
  p_layers->setColumnCount(2);
  p_layers->setColumnWidth(0, 50);
  p_layers->setColumnWidth(1, 200);
  p_layers->verticalHeader()->setVisible(false);
  p_layers->setRowCount(static_cast<int>(p_layout->libraries[0]->layers.size()));
  QStringList headers;
  headers << "Layer" << "Name";
  p_layers->setHorizontalHeaderLabels(headers);
  p_layers->horizontalHeaderItem(0)->setToolTip(tr("Layer"));
  p_layers->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignCenter);
  p_layers->horizontalHeaderItem(1)->setToolTip(tr("Name"));
  p_layers->horizontalHeaderItem(1)->setTextAlignment(Qt::AlignCenter);

  for (int i = 0; i < p_layout->libraries[0]->layers.size(); ++i) {
    QTableWidgetItem *p_item1 = new QTableWidgetItem(QString::number(p_layout->libraries[0]->layers[i].layer));
    p_item1->setFlags(p_item1->flags() & ~Qt::ItemIsEditable);
    p_item1->setTextAlignment(Qt::AlignCenter);
    p_layers->setItem(i, 0, p_item1);
    
    QTableWidgetItem *p_item2 = new QTableWidgetItem(QString::fromStdString(p_layout->libraries[0]->layers[i].name));
    p_layers->setItem(i, 1, p_item2);
   
  }

  p_vLayout->addWidget(p_layers);
  
  QDialogButtonBox *p_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  connect(p_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(p_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  p_vLayout->addWidget(p_buttonBox);
}

void Dlg_Map::accept() {
  for (int i = 0; i < p_layout->libraries[0]->layers.size(); ++i) {
    if (p_layout->libraries[0]->layers[i].name != p_layers->item(i, 1)->text().toStdString())
      p_layout->libraries[0]->layers[i].name = p_layers->item(i, 1)->text().toStdString();
  }

  QDialog::accept();
}

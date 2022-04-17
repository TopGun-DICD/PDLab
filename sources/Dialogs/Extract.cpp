#include "Extract.hpp"

#include <QVBoxLayout>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>

Dlg_Extract::Dlg_Extract(QWidget *parent, LayersInfo *li) : QDialog(parent), p_maskEdit(nullptr), p_selectedLayers(nullptr), p_layersInfo(li) {
  setWindowTitle(tr("Extract Layers"));
  resize(QSize(300, 400));

  QVBoxLayout *p_vLayout = new QVBoxLayout(this);
  
  if (!p_layersInfo->mask.isEmpty() && p_layersInfo->mask != "*") {
    p_vLayout->addWidget(new QLabel("Layers mask:", this));
    p_maskEdit = new QLineEdit(p_layersInfo->mask, this);
    p_vLayout->addWidget(p_maskEdit);
  }
  else {
    p_vLayout->addWidget(new QLabel("Layers info:", this));
    p_selectedLayers = new QListWidget(this);
    QListWidgetItem *p_item = nullptr;
    for (int i = 0; i < p_layersInfo->layers.size(); ++i) {
      p_item = new QListWidgetItem(p_layersInfo->layers[i].id + p_layersInfo->layers[i].name);
      p_item->setFlags(p_item->flags() | Qt::ItemIsUserCheckable);
      p_item->setCheckState(p_layersInfo->layers[i].selected ? Qt::Checked : Qt::Unchecked);
      p_selectedLayers->addItem(p_item);
    }
    p_vLayout->addWidget(p_selectedLayers);
  }
  
  QDialogButtonBox *p_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  connect(p_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(p_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  p_vLayout->addWidget(p_buttonBox);
}

void Dlg_Extract::accept() {
  if (!p_layersInfo)
    QDialog::reject();

  if (p_maskEdit) {
    p_layersInfo->mask = p_maskEdit->text();
    //TODO: Check this text to validity
  }
  if (p_selectedLayers) {
    int selectedItemsCounter = 0;
    p_layersInfo->selected.clear();
    for (int i = 0; i < p_layersInfo->layers.size(); ++i) {
      p_layersInfo->layers[i].selected = false;
      if (p_selectedLayers->item(i)->checkState() == Qt::Checked) {
        p_layersInfo->layers[i].selected = true;
        p_layersInfo->selected.push_back(p_layersInfo->layers[i].id.toInt());
        ++selectedItemsCounter;
      }
    }
    if (!selectedItemsCounter)
      return;
  }

  QDialog::accept();
}

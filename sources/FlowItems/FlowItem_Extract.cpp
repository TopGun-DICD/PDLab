#include "FlowItem_Extract.hpp"

#include <QPen>
#include <QPainter>
#include <QMessageBox>
#include <QFileInfo>
#include <QProcess>
#include <QTemporaryFile>
#include <QDateTime>

#include <ctime>

#include "../Logger.hpp"
#include "FlowItemPort.hpp"
#include "FlowItemConnection.hpp"

FlowItem_Extract::FlowItem_Extract(BasicLogger *logger) : FlowItem(FlowItemType::extract, QString("EXTRACT"), logger, LayoutOwnershipMode::make_copy) {
  AddInputPort();
  AddOutputPort();
  titleBgColor = QColor(246, 168, 0);
}

FlowItem_Extract::~FlowItem_Extract() { 
}

bool FlowItem_Extract::DropEventHandler() {
  p_logger->Log("'EXTRACT-DROP' was called");

  return true;
}

bool FlowItem_Extract::ExecuteEventHandler() {
  p_logger->Log("'EXTRACT-EXECUTE' was called");

  if (!inputPorts[0]->IsConnected()) {
    p_logger->Log("'EXTRACT-EXECUTE': input is not connected!");
    return false;
  }
  if (!p_resultLayout) {
    p_logger->Log("'EXTRACT-EXECUTE': input layout is nullptr");
    return false;
  }

  layersInfo.layers.clear();
  for (int i = 0; i < p_resultLayout->libraries[0]->layers.size(); ++i) {
    LayerInfo li;
    li.id = QString::number(p_resultLayout->libraries[0]->layers[i].layer);
    li.selected = false;
    layersInfo.layers.push_back(li);
  } 

  Dlg_Extract dlg(nullptr, &layersInfo);
  if (dlg.exec() == QDialog::Rejected)
    return false;

  //for (int i = 0; i < layersInfo.selected.size(); ++i)
  //  p_logger->Log(QString("Layer %1").arg(layersInfo.selected[i]));

  //TODO: ¬ыставить make_none и не удал€ть элементы, а вручную формировать только нужные - думаю, это будет быстрее
  for (int i = 0; i < p_resultLayout->libraries[0]->elements.size(); ++i) {
    for (int j = 0; j < p_resultLayout->libraries[0]->elements[i]->items.size(); ++j) {
      GeometryItem *p_item = p_resultLayout->libraries[0]->elements[i]->items[j];
      bool leaveItem = false;
      for (int k = 0; k < layersInfo.selected.size(); ++k)
        if (p_item->layer == layersInfo.selected[k]) {
          leaveItem = true;
          break;
        }
      if (leaveItem)
        continue;
      delete p_item;
      p_item = nullptr;
      p_resultLayout->libraries[0]->elements[i]->items.erase(p_resultLayout->libraries[0]->elements[i]->items.begin() + j);
      --j;
    }
    if (!p_resultLayout->libraries[0]->elements[i]->items.empty())
      continue;
    delete p_resultLayout->libraries[0]->elements[i];
    p_resultLayout->libraries[0]->elements.erase(p_resultLayout->libraries[0]->elements.begin() + i);
    --i;
  }

  for (int i = 0; i < p_resultLayout->libraries[0]->layers.size(); ++i) {
    bool leaveItem = false;
    for (int j = 0; j < layersInfo.selected.size(); ++j) {
      if (p_resultLayout->libraries[0]->layers[i].layer == layersInfo.selected[j]) {
        leaveItem = true;
        break;
      }
    }
    if (leaveItem)
      continue;
    p_resultLayout->libraries[0]->layers.erase(p_resultLayout->libraries[0]->layers.begin() + i);
    --i;
  }

  return true;
}

bool FlowItem_Extract::OpenResultsEventHandler() {
  p_logger->Log("'EXTRACT-OPENRESULTS' was called");
  return true;
}

bool FlowItem_Extract::ResetEventHandler() {
  p_logger->Log("'EXTRACT-RESET' was called");

  return true;
}

bool FlowItem_Extract::ShowPropertesEventHandler() {
  p_logger->Log("'EXTRACT-PROPERTIES' was called");

  return true;
}

QString FlowItem_Extract::GetInfoString() {
  return QString("Extract info string");
}
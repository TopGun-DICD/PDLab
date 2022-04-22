#include "FlowItem_Extract.hpp"

#include <QPen>
#include <QPainter>
#include <QMessageBox>
#include <QFileInfo>
#include <QProcess>
#include <QTemporaryFile>
#include <QDateTime>

#include <ctime>

#include "../Helper.hpp"
#include "../Config.hpp"
#include "../Logger.hpp"
#include "FlowItemPort.hpp"
#include "FlowItemConnection.hpp"

FlowItem_Extract::FlowItem_Extract(BasicLogger *logger) : FlowItem(FlowItemType::extract, QString("EXTRACT"), logger, LayoutOwnershipMode::make_copy) {
  AddInputPort(PortDataType::layout);
  AddOutputPort(PortDataType::layout);
  titleBgColor = Config::Get()->colors.headerLayerOperations;

  layersInfo.layers.clear();
  layersInfo.selected.clear();
  layersInfo.mask = "*";
}

FlowItem_Extract::~FlowItem_Extract() { 
}

bool FlowItem_Extract::DropEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'EXTRACT-DROP' was called");
#endif

  return true;
}

bool FlowItem_Extract::ExecuteEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'EXTRACT-EXECUTE' was called");
#endif

  // This check is performed by FlowItem::OnHandleEvent_Execute
  //if (!inputPorts[0]->IsConnected()) {
  //  p_logger->Log("'EXTRACT-EXECUTE': input is not connected!");
  //  return false;
  //}
  // // It is not possible!
  //if (!p_resultLayout) {
  //  p_logger->Log("'EXTRACT-EXECUTE': input layout is nullptr");
  //  return false;
  //}

  //layersInfo.layers.clear();
  for (int i = 0; i < p_resultLayout->libraries[0]->layers.size(); ++i) {
    LayerInfo li;
    li.id = QString::number(p_resultLayout->libraries[0]->layers[i].layer) + " (" + QString::number(p_resultLayout->libraries[0]->layers[i].dataType) + ")";
    if (!p_resultLayout->libraries[0]->layers[i].name.empty())
      li.name = QString(" [ %1 ]").arg(p_resultLayout->libraries[0]->layers[i].name.c_str());
    li.selected = false;
    layersInfo.layers.push_back(li);
  } 

  Dlg_Extract dlg(nullptr, &layersInfo);
  if (dlg.exec() == QDialog::Rejected)
    return false;

  //TODO: Set flag to make_none do not remove elements automatically, but do it manually only for necessary - I think it will be faster
  Element *p_element = nullptr;
  for (int i = 0; i < p_resultLayout->libraries[0]->elements.size(); ++i) {
    p_element = p_resultLayout->libraries[0]->elements[i];
    for (int j = 0; j < p_element->geometries.size(); ++j) {
      Geometry *p_geometry = p_element->geometries[j];
      bool leaveItem = false;
      if (p_geometry->type == GeometryType::reference) {
        Reference *p_ref = static_cast<Reference *>(p_geometry);
        for(size_t l = 0; l < p_ref->referenceTo->geometries.size(); ++l)
          for (int k = 0; k < layersInfo.selected.size(); ++k) {
            if (p_ref->referenceTo->geometries[l]->layer == layersInfo.selected[k]) {
              leaveItem = true;
              break;
            }
          }
      }
      else {
        for (int k = 0; k < layersInfo.selected.size(); ++k)
          if (p_geometry->layer == layersInfo.selected[k]) {
            leaveItem = true;
            break;
          }
      }
      if (leaveItem)
        continue;
      delete p_geometry;
      p_geometry = nullptr;
      p_element->geometries.erase(p_element->geometries.begin() + j);
      --j;
    }
    if (!p_element->geometries.empty())
      continue;
    delete p_element;
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
#if defined(DEBUG_PRINT)
  p_logger->Log("'EXTRACT-OPENRESULTS' was called");
#endif

  Helper::GetInstance()->ShowLayout(p_resultLayout);

  return true;
}

bool FlowItem_Extract::ResetEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'EXTRACT-RESET' was called");
#endif

  layersInfo.layers.clear();
  layersInfo.selected.clear();
  layersInfo.mask = "*";

  return true;
}

bool FlowItem_Extract::ShowPropertesEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'EXTRACT-PROPERTIES' was called");
#endif

  /*if (p_resultLayout) {
    if (!p_resultLayout->libraries.empty()) {
      for (int i = 0; i < p_resultLayout->libraries[0]->layers.size(); ++i) {
        LayerInfo li;
        li.id = QString::number(p_resultLayout->libraries[0]->layers[i].layer);
        if (!p_resultLayout->libraries[0]->layers[i].name.empty())
          li.name = QString(" [ %1 ]").arg(p_resultLayout->libraries[0]->layers[i].name.c_str());
        li.selected = false;
        layersInfo.layers.push_back(li);
      }
    }
  }

  Dlg_Extract dlg(nullptr, &layersInfo);
  if (dlg.exec() == QDialog::Rejected)
    return false;*/


  return true;
}

QString FlowItem_Extract::GetInfoString() {
  return QString("Extract info string");
}
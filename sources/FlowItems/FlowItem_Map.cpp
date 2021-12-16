#include "FlowItem_Map.hpp"

#include <ctime>

#include "../Logger.hpp"
#include "FlowItemPort.hpp"
#include "FlowItemConnection.hpp"
#include "../Dialogs/Map.hpp"
#include "../Config.hpp"

FlowItem_Map::FlowItem_Map(BasicLogger *logger) : FlowItem(FlowItemType::editdata, QString("MAP"), logger, LayoutOwnershipMode::make_link) {
  AddInputPort(PortDataType::layout);
  AddOutputPort(PortDataType::layout);
  titleBgColor = Config::Get()->colors.headerLayerOperations;
}

FlowItem_Map::~FlowItem_Map() {
}

bool FlowItem_Map::DropEventHandler() {
  p_logger->Log("'MAP-DROP' was called");
  
  return true;
}

bool FlowItem_Map::ExecuteEventHandler() {
  p_logger->Log("'MAP-EXECUTE' was called");

  Dlg_Map dlg(nullptr, p_resultLayout);
  
  return (dlg.exec() == QDialog::Accepted);
}

bool FlowItem_Map::OpenResultsEventHandler() {
  p_logger->Log("'MAP-OPENRESULTS' was called");

  return true;
}

bool FlowItem_Map::ResetEventHandler() {
  p_logger->Log("'MAP-RESET' was called");

  return true;
}

bool FlowItem_Map::ShowPropertesEventHandler() {
  p_logger->Log("'MAP-PROPERTIES' was called");

  return true;
}

QString FlowItem_Map::GetInfoString() {
  return QString("MAP info string");
}
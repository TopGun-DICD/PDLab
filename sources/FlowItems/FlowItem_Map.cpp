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
#if defined(DEBUG_PRINT)
  p_logger->Log("'MAP-DROP' was called");
#endif

  return true;
}

bool FlowItem_Map::ExecuteEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'MAP-EXECUTE' was called");
#endif

  Dlg_Map dlg(nullptr, p_resultLayout);
  
  return (dlg.exec() == QDialog::Accepted);
}

bool FlowItem_Map::OpenResultsEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'MAP-OPENRESULTS' was called");
#endif

  return true;
}

bool FlowItem_Map::ResetEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'MAP-RESET' was called");
#endif

  return true;
}

bool FlowItem_Map::ShowPropertiesEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'MAP-PROPERTIES' was called");
#endif

  return true;
}

QString FlowItem_Map::GetInfoString() {
  return QString("MAP info string");
}
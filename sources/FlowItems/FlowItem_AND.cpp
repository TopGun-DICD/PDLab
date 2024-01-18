#include "FlowItem_AND.hpp"

#include <ctime>

#include "../Logger.hpp"
#include "FlowItemPort.hpp"
#include "FlowItemConnection.hpp"
#include "../Config.hpp"

FlowItem_AND::FlowItem_AND(BasicLogger *logger) : FlowItem(FlowItemType::AND, QString("AND"), logger, LayoutOwnershipMode::make_copy) {
  AddInputPort(PortDataType::layout);
  AddInputPort(PortDataType::layout);
  AddOutputPort(PortDataType::layout);
  titleBgColor = Config::Get()->colors.headerImportExport;
  //bottomString = QString("KLayout");
}

FlowItem_AND::~FlowItem_AND() {
}

bool FlowItem_AND::DropEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'AND-DROP' was called");
#endif
  
  return true;
}

bool FlowItem_AND::ExecuteEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'AND-EXECUTE' was called");
#endif

  return true;
}

bool FlowItem_AND::OpenResultsEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'AND-OPENRESULTS' was called");
#endif
  return true;
}

bool FlowItem_AND::ResetEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'AND-RESET' was called");
#endif

  return true;
}

bool FlowItem_AND::ShowPropertiesEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'AND-PROPERTIES' was called");
#endif

  return true;
}

QString FlowItem_AND::GetInfoString() {
  return QString("AND info string");
}
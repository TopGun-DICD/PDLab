#include "FlowItem_Heal.hpp"

#include <ctime>

#include "../Logger.hpp"
#include "FlowItemPort.hpp"
#include "FlowItemConnection.hpp"
#include "../Config.hpp"

FlowItem_Heal::FlowItem_Heal(BasicLogger *logger) : FlowItem(FlowItemType::heal, QString("HEAL"), logger, LayoutOwnershipMode::make_copy) {
  AddInputPort(PortDataType::layout);
  AddOutputPort(PortDataType::layout);
  titleBgColor = Config::Get()->colors.headerLayoutOperations;
  //bottomString = QString("KLayout");
}

FlowItem_Heal::~FlowItem_Heal() {
}

bool FlowItem_Heal::DropEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'HEAL-DROP' was called");
#endif

  return true;
}

bool FlowItem_Heal::ExecuteEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'HEAL-EXECUTE' was called");
#endif

  return true;
}

bool FlowItem_Heal::OpenResultsEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'HEAL-OPENRESULTS' was called");
#endif
  return true;
}

bool FlowItem_Heal::ResetEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'HEAL-RESET' was called");
#endif

  return true;
}

bool FlowItem_Heal::ShowPropertesEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'HEAL-PROPERTIES' was called");
#endif

  return true;
}

QString FlowItem_Heal::GetInfoString() {
  return QString("Heal info string");
}
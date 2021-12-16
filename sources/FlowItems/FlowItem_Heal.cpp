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
  p_logger->Log("'HEAL-DROP' was called");
  
  return true;
}

bool FlowItem_Heal::ExecuteEventHandler() {
  p_logger->Log("'HEAL-EXECUTE' was called");

  return true;
}

bool FlowItem_Heal::OpenResultsEventHandler() {
  p_logger->Log("'HEAL-OPENRESULTS' was called");
  return true;
}

bool FlowItem_Heal::ResetEventHandler() {
  p_logger->Log("'HEAL-RESET' was called");

  return true;
}

bool FlowItem_Heal::ShowPropertesEventHandler() {
  p_logger->Log("'HEAL-PROPERTIES' was called");

  return true;
}

QString FlowItem_Heal::GetInfoString() {
  return QString("Heal info string");
}
#include "FlowItem_AND.hpp"

#include <ctime>

#include "../Logger.hpp"
#include "FlowItemPort.hpp"
#include "FlowItemConnection.hpp"

FlowItem_AND::FlowItem_AND(BasicLogger *logger) : FlowItem(FlowItemType::AND, QString("AND"), logger, LayoutOwnershipMode::make_copy) {
  AddInputPort(PortDataType::layout);
  AddInputPort(PortDataType::layout);
  AddOutputPort(PortDataType::layout);
  titleBgColor = QColor(139, 55, 78);
  //bottomString = QString("KLayout");
}

FlowItem_AND::~FlowItem_AND() {
}

bool FlowItem_AND::DropEventHandler() {
  p_logger->Log("'AND-DROP' was called");
  
  return true;
}

bool FlowItem_AND::ExecuteEventHandler() {
  p_logger->Log("'AND-EXECUTE' was called");

  return true;
}

bool FlowItem_AND::OpenResultsEventHandler() {
  p_logger->Log("'AND-OPENRESULTS' was called");
  return true;
}

bool FlowItem_AND::ResetEventHandler() {
  p_logger->Log("'AND-RESET' was called");

  return true;
}

bool FlowItem_AND::ShowPropertesEventHandler() {
  p_logger->Log("'AND-PROPERTIES' was called");

  return true;
}

QString FlowItem_AND::GetInfoString() {
  return QString("AND info string");
}
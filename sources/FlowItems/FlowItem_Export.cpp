#include "FlowItem_Export.hpp"

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

FlowItem_Export::FlowItem_Export(BasicLogger *logger) : FlowItem(FlowItemType::importdata, QString("EXPORT"), logger, LayoutOwnershipMode::make_nothing) {
  AddInputPort();
  titleBgColor = QColor(255, 0, 0);
}

FlowItem_Export::~FlowItem_Export() {
}


bool FlowItem_Export::DropEventHandler() {
  p_logger->Log("'EXPORT-DROP' was called");
  
  return true;
}

bool FlowItem_Export::ExecuteEventHandler() {
  p_logger->Log("'EXPORT-EXECUTE' was called");

  return true;
}

bool FlowItem_Export::OpenResultsEventHandler() {
  p_logger->Log("'EXPORT-OPENRESULTS' was called");

  return true;
}

bool FlowItem_Export::ResetEventHandler() {
  p_logger->Log("'EXPORT-RESET' was called");

  return true;
}

bool FlowItem_Export::ShowPropertesEventHandler() {
  p_logger->Log("'EXPORT-PROPERTIES' was called");

  return true;
}

QString FlowItem_Export::GetInfoString() {
  return QString("Export info string");
}
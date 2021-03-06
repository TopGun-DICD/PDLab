#include "FlowItem_Edit.hpp"

#include <QPen>
#include <QPainter>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QTemporaryFile>
#include <QDateTime>

#include <ctime>
#include <fstream>

#include "../Logger.hpp"
#include "FlowItemPort.hpp"
#include "FlowItemConnection.hpp"
#include "LayoutLoader.hpp"

const QString flowItemName("EDIT");

FlowItem_Edit::FlowItem_Edit(BasicLogger *logger) : FlowItem(FlowItemType::editdata, flowItemName, logger, LayoutOwnershipMode::make_new) {
  AddOutputPort(PortDataType::layout);
  titleBgColor = QColor(255, 0, 0);
  bottomString = QString("KLayout");
}

FlowItem_Edit::~FlowItem_Edit() {
  if(!tempLayoutFileName.isEmpty())
    if (QFile::exists(tempLayoutFileName))
      QFile::remove(tempLayoutFileName);
}

bool FlowItem_Edit::DropEventHandler() {
  p_logger->Log(QString("'%1-DROP' was called").arg(flowItemName));
  
  QTemporaryFile tempFile;
  if (!tempFile.open()) {
    p_logger->Error(QString("'%1-DROP' : Can't create temp file for layout"));
    return false;
  }
  tempFile.close();

  tempFile.setAutoRemove(false);
  tempLayoutFileName = tempFile.fileName();
  p_logger->Log(QString("Temp file name : %1").arg(tempLayoutFileName));
  if (!QFile::exists("templates/Empty.bin.gds")) {
    p_logger->Error(QString("'%1-DROP' : Can't find template layout").arg(flowItemName));
    return false;
  }

  std::ifstream src("templates/Empty.bin.gds", std::ios::binary);
  std::ofstream dst(tempLayoutFileName.toStdWString(), std::ios::binary);
  dst << src.rdbuf();
  src.close();
  dst.close();

  QFileInfo fi(tempLayoutFileName);
  bottomString = fi.fileName();

  return true;
}

bool FlowItem_Edit::ExecuteEventHandler() {
  p_logger->Log(QString("'%1-EXECUTE' was called").arg(flowItemName));

  

  return true;
}

bool FlowItem_Edit::OpenResultsEventHandler() {
  p_logger->Log(QString("'%1-OPENRESULTS' was called").arg(flowItemName));
  return true;
}

bool FlowItem_Edit::ResetEventHandler() {
  p_logger->Log(QString("'%1-RESET' was called").arg(flowItemName));
  itemStatus = FlowItemStatus::unknown;

  return true;
}

bool FlowItem_Edit::ShowPropertesEventHandler() {
  p_logger->Log(QString("'%1-PROPERTIES' was called").arg(flowItemName));

  return true;
}

QString FlowItem_Edit::GetInfoString() {
  return QString("Edit info string");
}
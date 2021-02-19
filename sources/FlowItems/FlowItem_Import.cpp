#include "FlowItem_Import.hpp"

#include <QFileInfo>
#include <QDateTime>
#include <QFileDialog>

#include <ctime>

#include "../Logger.hpp"
#include "FlowItemPort.hpp"
#include "FlowItemConnection.hpp"
#include "../Dialogs/Import.hpp"
#include "LayoutLoader.hpp"

FlowItem_Import::FlowItem_Import(BasicLogger *logger) : FlowItem(FlowItemType::importdata, QString("IMPORT"), logger, LayoutOwnershipMode::make_new) {
  AddOutputPort();
  titleBgColor = QColor(255, 0, 0);
}

FlowItem_Import::~FlowItem_Import() {
}

bool FlowItem_Import::DropEventHandler() {
  p_logger->Log("'IMPORT-DROP' was called");

  Dlg_Import dlg(nullptr);
  if (!dlg.exec())
    return false;

  fileName = dlg.p_fileLayout->text();

  QFileInfo fi(fileName);
  topString = "???";
  QString fileSuffix = fi.suffix().toLower();
  if (fileSuffix == "gds" || fileSuffix == "gdsii")
    topString = "GDSII";
  if (fileSuffix == "msk")
    topString = "MSK";
  fileShortName = fi.fileName();
  fileSize = fi.size();

  bottomString = fi.fileName();

  return true;
}

bool FlowItem_Import::ExecuteEventHandler() {
  p_logger->Log("'IMPORT-EXECUTE' was called");

  std::clock_t timeB = std::clock();
  LayoutLoader::GetInstance()->ReadLayoutFromFile(fileName, p_resultLayout);
  std::clock_t timeC = std::clock();
  if (!p_resultLayout) {
    p_logger->Error(QString("Failed to read layout from '%1'").arg(fileName));
    return false;
  }
  p_logger->Log(QString("Input file '%1' (GDSII format) readed successfully.").arg(fileName));
  p_logger->Log(QString("Layout read in %2 ms.").arg(timeC - timeB));

  return true;
}

bool FlowItem_Import::OpenResultsEventHandler() {
  p_logger->Log("'IMPORT-OPENRESULTS' was called");
  return true;
}

bool FlowItem_Import::ResetEventHandler() {
  p_logger->Log("'IMPORT-RESET' was called");

  //LayoutLoader::GetInstance()->FreeLayout(&outputPorts[0]->p_layout);

  return true;
}

bool FlowItem_Import::ShowPropertesEventHandler() {
  p_logger->Log("'IMPORT-PROPERTIES' was called");

  return true;
}

QString FlowItem_Import::GetInfoString() {
  return QString("File name: %1\nFile type: %2 (based on extension)\nFile size: %3").arg(fileShortName).arg(topString).arg(fileSize);
}

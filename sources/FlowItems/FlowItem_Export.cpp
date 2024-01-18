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
#include "../Dialogs/Export.hpp"
#include "LayoutLoader.hpp"
#include "../Helper.hpp"
#include "../Config.hpp"

FlowItem_Export::FlowItem_Export(BasicLogger *logger) : FlowItem(FlowItemType::importdata, QString("EXPORT"), logger, LayoutOwnershipMode::make_link) {
  AddInputPort(PortDataType::layout);
  titleBgColor = Config::Get()->colors.headerImportExport;
}

FlowItem_Export::~FlowItem_Export() {
}


bool FlowItem_Export::DropEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'EXPORT-DROP' was called");
#endif

  Dlg_Export dlg(nullptr);
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

bool FlowItem_Export::ExecuteEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'EXPORT-EXECUTE' was called");
#endif

  std::clock_t timeB = std::clock();
  bool retCode = LayoutLoader::GetInstance()->WriteLayout(p_resultLayout, fileName.toStdWString(), FileFormat::GDSII_bin);
  std::clock_t timeC = std::clock();
  if (!retCode) {
    p_logger->Error(QString("Failed to write layout to '%1'").arg(fileName));
    return false;
  }
  p_logger->Log(QString("Output file '%1' (GDSII format) written successfully.").arg(fileName));
  p_logger->Log(QString("Layout writing took %2 ms.").arg(timeC - timeB));


  return true;
}

bool FlowItem_Export::OpenResultsEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'EXPORT-OPENRESULTS' was called");
#endif

  return true;
}

bool FlowItem_Export::ResetEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'EXPORT-RESET' was called");
#endif

  return true;
}

bool FlowItem_Export::ShowPropertiesEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'EXPORT-PROPERTIES' was called");
#endif

  return true;
}

QString FlowItem_Export::GetInfoString() {
  return QString("Export info string");
}
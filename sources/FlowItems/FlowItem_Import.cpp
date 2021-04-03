#include "FlowItem_Import.hpp"

#include <QFileInfo>
#include <QDateTime>
#include <QFileDialog>
#include <QTextStream>

#include <ctime>

#include "../Logger.hpp"
#include "FlowItemPort.hpp"
#include "FlowItemConnection.hpp"
#include "../Dialogs/Import.hpp"
#include "LayoutLoader.hpp"
#include "../Helper.hpp"

FlowItem_Import::FlowItem_Import(BasicLogger *logger) : FlowItem(FlowItemType::importdata, QString("IMPORT"), logger, LayoutOwnershipMode::make_new) {
  AddOutputPort(PortDataType::layout);
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

  if (!dlg.p_fileMapping->text().isEmpty())
    techFileName = dlg.p_fileMapping->text();

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

  if (!techFileName.isEmpty()) {
    QFile techFile;
    techFile.setFileName(techFileName);
    if (!techFile.open(QFile::ReadOnly)) {
      p_logger->Error(QString("Can't open tech file '%1'. Layers will not be mapped.").arg(techFileName));
      return true;
    }

    QTextStream in(&techFile);
    QString line;
    while (!in.atEnd()) {

      in >> line;
      while (line != "layerDefinitions(" && !in.atEnd())
        in >> line;
      if (in.atEnd())
        break;

      in >> line;
      while (line != "techLayers(" && !in.atEnd())
        in >> line;
      if (in.atEnd())
        break;

      while (line != ")" && !in.atEnd()) {
        line = in.readLine().trimmed();
        if (line[0] == ';')
          continue;
        if (line.isEmpty())
          continue;
        QStringList tokens = line.split(' ', QString::SkipEmptyParts);
        if (tokens.size() != 5)
          continue;
        if (tokens.at(0) != "(" || tokens.at(4) != ")")
          continue;
        QString layerName = tokens.at(1);
        int     layerId   = tokens.at(2).toInt();

        for (size_t i = 0; i < p_resultLayout->libraries[0]->layers.size(); ++i)
          if (p_resultLayout->libraries[0]->layers[i].layer == layerId)
            p_resultLayout->libraries[0]->layers[i].name = layerName.toStdString();
      }
      if (in.atEnd())
        break;

    }

    techFile.close();
  }

  return true;
}

bool FlowItem_Import::OpenResultsEventHandler() {
  p_logger->Log("'IMPORT-OPENRESULTS' was called");

  Helper::GetInstance()->ShowLayout(p_resultLayout);

  return true;
}

bool FlowItem_Import::ResetEventHandler() {
  p_logger->Log("'IMPORT-RESET' was called");

  
  return true;
}

bool FlowItem_Import::ShowPropertesEventHandler() {
  p_logger->Log("'IMPORT-PROPERTIES' was called");

  Dlg_Import dlg(nullptr);
  dlg.p_fileLayout->setText(fileName);
  if (!techFileName.isEmpty())
    dlg.p_fileMapping->setText(fileName);
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

  if (!dlg.p_fileMapping->text().isEmpty())
    techFileName = dlg.p_fileMapping->text();

  return true;


  return true;
}

QString FlowItem_Import::GetInfoString() {
  return QString("File name: %1\nFile type: %2 (based on extension)\nFile size: %3").arg(fileShortName).arg(topString).arg(fileSize);
}

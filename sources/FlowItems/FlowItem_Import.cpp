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
#include "../Config.hpp"

FlowItem_Import::FlowItem_Import(BasicLogger *logger) : FlowItem(FlowItemType::importdata, QString("IMPORT"), logger, LayoutOwnershipMode::make_new) {
  AddOutputPort(PortDataType::layout);
  titleBgColor = Config::Get()->colors.headerImportExport;
}

FlowItem_Import::~FlowItem_Import() {
}

bool FlowItem_Import::DropEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'IMPORT-DROP' was called");
#endif

  Dlg_Import dlg(nullptr);
  if (!dlg.exec())
    return false;

  return InitDataFromDialogCall(dlg);
}

bool FlowItem_Import::ExecuteEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'IMPORT-EXECUTE' was called");
#endif

  std::clock_t timeB = std::clock();
  LayoutLoader::GetInstance()->ReadLayoutFromFile(fileName, p_resultLayout);
  std::clock_t timeC = std::clock();
  if (p_resultLayout->libraries.empty()) {
    p_logger->Error(QString("Failed to read layout from '%1'").arg(fileName));
    return false;
  }
  switch (p_resultLayout->fileFormat) {
    case FileFormat::GDSII_bin:
      topString = "GDSII";
      formatInfoString = topString + " binary";
      break;
    case FileFormat::GDSII_ascii:
      topString = "GDSII";
      formatInfoString = topString + " ASCII";
      break;
    case FileFormat::MSK:
      topString = "MSK";
      formatInfoString = "Microwind " + topString;
      break;
  }

  p_logger->Log(QString("Input file '%1' (%2 format) was read successfully.").arg(fileName).arg(topString));
  p_logger->Log(QString("Layout read in %1 ms.").arg(timeC - timeB));

  ApplyTechFileData();

  return true;
}

bool FlowItem_Import::OpenResultsEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'IMPORT-OPENRESULTS' was called");
#endif

  Helper::GetInstance()->ShowLayout(p_resultLayout);

  return true;
}

bool FlowItem_Import::ResetEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'IMPORT-RESET' was called");
#endif

  return true;
}

bool FlowItem_Import::ShowPropertesEventHandler() {
#if defined(DEBUG_PRINT)
  p_logger->Log("'IMPORT-PROPERTIES' was called");
#endif

  LayoutReaderOptions &options = LayoutLoader::GetInstance()->GetOptions();

  Dlg_Import dlg(nullptr);
  dlg.p_fileLayout->setText(fileName);
  if (!techFileName.isEmpty())
    dlg.p_fileMapping->setText(techFileName);
  if (selectedLayers != "all")
    dlg.p_layers2Read->setText(selectedLayers);
  if (ignoredLayers != "none")
    dlg.p_layers2Ignore->setText(ignoredLayers);
  dlg.p_cbConvertBoundaries->setChecked(options.convertPolysToRects);
  dlg.p_cbIgnoreZeroWidthPath->setChecked(options.ignoreZeroWidthPaths);

  if (!dlg.exec())
    return false;

  return InitDataFromDialogCall(dlg);
}

QString FlowItem_Import::GetInfoString() {
  if(selectedLayers == "all")
    if(ignoredLayers == "none")
      return QString("File name: %1\nFile type: %2\nFile size: %3\nLayers: %4").arg(fileShortName).arg(formatInfoString).arg(fileSize).arg(selectedLayers);
    else
      return QString("File name: %1\nFile type: %2\nFile size: %3\nLayers: %4 except %5").arg(fileShortName).arg(formatInfoString).arg(fileSize).arg(selectedLayers).arg(ignoredLayers);
  else
    return QString("File name: %1\nFile type: %2\nFile size: %3\nLayers: %4").arg(fileShortName).arg(formatInfoString).arg(fileSize).arg(selectedLayers).arg(ignoredLayers);
}

bool FlowItem_Import::InitDataFromDialogCall(const Dlg_Import& dlg) {
  fileName = dlg.p_fileLayout->text();

  LayoutReaderOptions &options = LayoutLoader::GetInstance()->GetOptions();

  options.convertPolysToRects = dlg.p_cbConvertBoundaries->isChecked();
  options.ignoreZeroWidthPaths = dlg.p_cbIgnoreZeroWidthPath->isChecked();
  options.layersToRead.clear();
  options.layersToIgnore.clear();

  QFileInfo fi(fileName);
  topString = formatInfoString = "???";
  QString fileSuffix = fi.suffix().toLower();
  if (fileSuffix == "gds" || fileSuffix == "gdsii" || fileSuffix == "gds2") {
    topString = "GDSII";
    formatInfoString = topString + " (based on extension)";
  }
  if (fileSuffix == "msk") {
    topString = "MSK";
    formatInfoString = topString + " (based on extension)";
  }
  fileShortName = fi.fileName();
  fileSize = fi.size();

  bottomString = fi.fileName();

  if (!dlg.p_fileMapping->text().isEmpty())
    techFileName = dlg.p_fileMapping->text();

  selectedLayers = "all";
  ignoredLayers = "none";

  if (!dlg.p_layers2Ignore->text().isEmpty() && dlg.p_layers2Ignore->text().trimmed() != "none") {
    ParseLayersString(dlg.p_layers2Ignore->text(), options.layersToIgnore);
    ignoredLayers.clear();
    for (int i = 0; i < options.layersToIgnore.size() - 1; ++i)
      ignoredLayers += QString::number(options.layersToIgnore[i]) + ", ";
    ignoredLayers += QString::number(options.layersToIgnore.back());
  }

  if (!dlg.p_layers2Read->text().isEmpty() && dlg.p_layers2Read->text().trimmed() != "*") {
    ParseLayersString(dlg.p_layers2Read->text(), options.layersToRead);

    if (!options.layersToIgnore.empty()) {
      for (int i = 0; i < options.layersToRead.size(); ++i)
        for (int j = 0; j < options.layersToIgnore.size(); ++j)
          if (options.layersToRead[i] == options.layersToIgnore[j]) {
            options.layersToRead.erase(options.layersToRead.begin() + i);
            --i;
          }
    }

    ignoredLayers = "none";
    options.layersToIgnore.clear();

    selectedLayers.clear();
    for (int i = 0; i < options.layersToRead.size() - 1; ++i)
      selectedLayers += QString::number(options.layersToRead[i]) + ", ";
    selectedLayers += QString::number(options.layersToRead.back());
  }
  return true;
}

bool FlowItem_Import::ParseLayersString(const QString& str, std::vector<int>& vec) {
  QStringList sl = str.split(',', QString::SkipEmptyParts);
  for (int i = 0; i < sl.size(); ++i) {
    int pos = sl[i].indexOf("-");
    if (pos == -1)
      vec.push_back(sl[i].trimmed().toInt());
    else {
      QStringList sl2 = sl[i].split('-', QString::SkipEmptyParts);
      int n1 = sl2[0].toInt();
      int n2 = sl2[1].toInt();
      if (n1 > n2) {
        n1 += n2;
        n2 = n1 - n2;
        n1 -= n2;
      }
      for (int j = n1; j <= n2; ++j)
        vec.push_back(j);
    }
  }
  return true;
}

void FlowItem_Import::ApplyTechFileData() {
  if (techFileName.isEmpty())
    return;
  QFile techFile;
  techFile.setFileName(techFileName);
  if (!techFile.open(QFile::ReadOnly)) {
    p_logger->Error(QString("Can't open tech file '%1'. Layers will not be mapped.").arg(techFileName));
    return;
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
      int     layerId = tokens.at(2).toInt();

      for (size_t i = 0; i < p_resultLayout->libraries[0]->layers.size(); ++i)
        if (p_resultLayout->libraries[0]->layers[i].layer == layerId)
          p_resultLayout->libraries[0]->layers[i].name = layerName.toStdString();
    }
    if (in.atEnd())
      break;
  }
  techFile.close();
}

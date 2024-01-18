#include "Config.hpp"

Config *Config::p_instance = nullptr;

ColorConfig::ColorConfig() {
  // Common
  bgFlowItem.setNamedColor("#F3F4F7"); // = QColor(152, 152, 152);
  borderFlowItem = QColor(50, 54, 68);
  
  selectedBorderFlowItem.setNamedColor("#FFFFFF");

  // Header colors
  headerImportExport.setNamedColor("#D35E35"); 
  headerLayerOperations.setNamedColor("#FFA600"); 
  headerLayoutOperations.setNamedColor("#58508D");
  headerUserDefined.setNamedColor("#58508D");

  // Port colors
  portLayoutEmpty.setNamedColor("#008A20");
  portLayoutConnected.setNamedColor("#00450C");
  portLefEmpty.setNamedColor("#4A3200");
  portLefConnected.setNamedColor("#996800");
  portDefEmpty.setNamedColor("#135E96");
  portDefConnected.setNamedColor("#043959");
  portVerilogEmpty.setNamedColor("#D63638");
  portVerilogConnected.setNamedColor("#691C1C");
}

Config::Config() : versionString("v0.0.3.15"), layoutEditorPath("C:\\Program Files (x86)\\KLayout\\klayout_app.exe") {
}

void Config::Init() {
  if (p_instance != nullptr)
    return;
  p_instance = new Config;
}

Config *Config::Get() {
  return p_instance;
}

void Config::Uninit() {
  if (nullptr == p_instance)
    return;
  delete p_instance;
  p_instance = nullptr;
}

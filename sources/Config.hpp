#pragma once

#include <QtGui/QColor>

// Version enumeration (see Config::versionString):
// {Release} . {Debug} . {Concept Idea} . {Small Feature and Bug Fix} 
// {Concept Idea}s:
// = 1 - Flow operations
// = 2 - User Flow operations separated from main application (implemented as DLLs)
// = 3 - Layout viewer
// ? 4 - Verlog, LEF, DEF, separate inputs and outputs
// ? 5 - 

//#define DEBUG_PRINT

struct ColorConfig {
  // Common
  QColor  bgFlowItem;
  QColor  borderFlowItem,
          selectedBorderFlowItem;
  // Header colors
  QColor  headerImportExport,
          headerLayerOperations,
          headerLayoutOperations,
          headerUserDefined;
  // Port colors
  QColor  portLayoutEmpty,
          portLayoutConnected;
  QColor  portLefEmpty,
          portLefConnected;
  QColor  portDefEmpty,
          portDefConnected;
  QColor  portVerilogEmpty,
          portVerilogConnected;
public:
  ColorConfig();
};

class Config {
private:
  static Config  *p_instance;
private:
  Config();
public:
  static void     Init();
  static Config  *Get();
  static void     Uninit();
public:
  ColorConfig     colors;
  QString         versionString;
  QString         layoutEditorPath;
};
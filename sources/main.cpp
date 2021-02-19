#include <QApplication>
#include <QResource>

#include "MainWindow.hpp"
#include "Logger.hpp"
#include "FlowItems/LayoutLoader.hpp"
#include "Logger.hpp"

int __stdcall WinMain(HINSTANCE, HINSTANCE, char*, int) {
  int     argc = 0;
  char  **argv = nullptr;

  QResource::registerResource("PDLab.rcc");

  QStringList paths = QCoreApplication::libraryPaths();
  paths.append(".");
  paths.append("plugins");
  paths.append("platforms");
  QCoreApplication::setLibraryPaths(paths);

  QApplication app(argc, argv);

  Logger *p_logger = new Logger;

  MainWindow *p_wnd = new MainWindow(p_logger);
  LayoutLoader::Init(p_logger);
  p_wnd->show();

  int retCode = app.exec();

  delete p_logger;
  p_logger = nullptr;
  LayoutLoader::Free();

  return retCode;
}

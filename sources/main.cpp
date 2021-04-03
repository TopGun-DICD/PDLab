#include <QApplication>
#include <QResource>

#include "MainWindow.hpp"
#include "Logger.hpp"
#include "FlowItems/LayoutLoader.hpp"
#include "Logger.hpp"

#if defined(_WIN32)
int __stdcall WinMain(HINSTANCE, HINSTANCE, char*, int) {
  int     argc = 0;
  char  **argv = nullptr;
#else
int main(int argc, char **argv) {
#endif
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

  LayoutLoader::Free();
  delete p_logger;
  p_logger = nullptr;

  return retCode;
}

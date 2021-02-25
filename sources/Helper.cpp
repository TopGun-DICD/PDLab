#include "Helper.hpp"

#include "MainWindow.hpp"

Helper *Helper::p_instance = nullptr;

void Helper::Init() {
  if (!p_instance)
    p_instance = new Helper;
}

void Helper::Free() {
  if (p_instance) {
    delete p_instance;
    p_instance = nullptr;
  }
}

Helper *Helper::GetInstance() {
  return p_instance;
}

void Helper::AssignMainWindow(QMainWindow *window) {
  p_mainWindow = window;
}

void Helper::ShowLayout(Layout *layout) {
  if (!layout)
    return;
  static_cast<MainWindow *>(p_mainWindow)->ShowLayout(layout);
}

#include "LayoutLoader.hpp"

#include <ctime>

LayoutLoader *LayoutLoader::p_instance = nullptr;

LayoutLoader::LayoutLoader(BasicLogger *logger) : p_logger(logger), func_CreateLayout(nullptr), func_ReadLayout(nullptr), func_ClearLayout(nullptr), func_CopyLayout(nullptr), func_FreeLayout(nullptr) {
  std::clock_t timeA = std::clock();
#if defined _DEBUG
  layoutLibrary.setFileName("PDLab_DLL_Layoutd.dll");
#else
  layoutLibrary.setFileName("PDLab_DLL_Layout.dll");
#endif
  if (!layoutLibrary.load()) {
    p_logger->Error(QString("\tCan't load library 'PDLab_DLL_Layout.dll'"));
    return;
  }

  func_CreateLayout = (Func_CreateLayout)layoutLibrary.resolve("CreateLayoutFromFile");
  if (!func_CreateLayout) {
    p_logger->Error(QString("\tLibrary 'PDLab_DLL_Layout.dll' does not contain required function to create layout."));
    layoutLibrary.unload();
    return;
  }
  func_ReadLayout = (Func_ReadLayout)layoutLibrary.resolve("ReadLayoutFromFile");
  if (!func_ReadLayout) {
    p_logger->Error(QString("\tLibrary 'PDLab_DLL_Layout.dll' does not contain required function to load layout."));
    layoutLibrary.unload();
    return;
  }
  func_ClearLayout = (Func_ClearLayout)layoutLibrary.resolve("ClearLayout");
  if (!func_ClearLayout) {
    p_logger->Error(QString("\tLibrary 'PDLab_DLL_Layout.dll' does not contain required function to clear layout."));
    layoutLibrary.unload();
    return;
  }
  func_CopyLayout = (Func_CopyLayout)layoutLibrary.resolve("CopyLayout");
  if (!func_CopyLayout) {
    p_logger->Error(QString("\tLibrary 'PDLab_DLL_Layout.dll' does not contain required function to copy layout."));
    layoutLibrary.unload();
    return;
  }
  func_FreeLayout = (Func_FreeLayout)layoutLibrary.resolve("FreeLayout");
  if (!func_FreeLayout) {
    p_logger->Error(QString("\tLibrary 'PDLab_DLL_Layout.dll' does not contain required functions to free layout."));
    layoutLibrary.unload();
    return;
  }

}

LayoutLoader::~LayoutLoader() {
  if (layoutLibrary.isLoaded()) {
    layoutLibrary.unload();
  }
}

void LayoutLoader::Init(BasicLogger *logger) {
  if (p_instance)
  {
    logger->Warning("Layout loader was already initialized!!!");
    return;
  }
  p_instance = new LayoutLoader(logger);
}

void LayoutLoader::Free() {
  delete p_instance;
  p_instance = nullptr;
}

LayoutLoader *LayoutLoader::GetInstance() {
  return p_instance;
}

Layout *LayoutLoader::CreateLayoutFromFile(QString fileName) {
  if (!layoutLibrary.isLoaded()) {
    p_logger->Error("PDLab_DLL_Layout.dll is not loaded but 'CreateLayoutFromFile' function called.");
    return nullptr;
  }
  if (!func_CreateLayout) {
    p_logger->Error("func_ReadLayout is NULL but 'CreateLayoutFromFile' function called.");
    return nullptr;
  }
  return func_CreateLayout(fileName.toStdWString());
}

void LayoutLoader::ReadLayoutFromFile(QString fileName, Layout *layout) {
  if (!layoutLibrary.isLoaded()) {
    p_logger->Error("PDLab_DLL_Layout.dll is not loaded but 'CreateLayoutFromFile' function called.");
    return;
  }
  if (!func_ReadLayout) {
    p_logger->Error("func_ReadLayout is NULL but 'CreateLayoutFromFile' function called.");
    return;
  }
  func_ReadLayout(fileName.toStdWString(), layout);
}

void LayoutLoader::ClearLayout(Layout *layout) {
  if (!layoutLibrary.isLoaded()) {
    p_logger->Error("PDLab_DLL_Layout.dll is not loaded but 'ClearLayout' function called.");
    return;
  }
  if (!func_ClearLayout) {
    p_logger->Error("func_ClearLayout is NULL but 'ClearLayout' function called.");
    return;
  }
  func_ClearLayout(layout);
}

void LayoutLoader::CopyLayout(Layout *src, Layout *dst) {
  if (!layoutLibrary.isLoaded()) {
    p_logger->Error("PDLab_DLL_Layout.dll is not loaded but 'CopyLayout' function called.");
    return;
  }
  if (!func_CopyLayout) {
    p_logger->Error("func_CopyLayout is NULL but 'CopyLayout' function called.");
    return;
  }
  func_CopyLayout(src, dst);
}

void LayoutLoader::FreeLayout(Layout **layout) {
  if (!layoutLibrary.isLoaded()) {
    p_logger->Error("PDLab_DLL_Layout.dll is not loaded but 'FreeLayout' function called.");
    return;
  }
  if (!func_FreeLayout) {
    p_logger->Error("func_FreeLayout is NULL but 'FreeLayout' function called.");
    return;
  }
  func_FreeLayout(layout);
}

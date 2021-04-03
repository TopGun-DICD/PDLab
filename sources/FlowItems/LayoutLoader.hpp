#pragma once

#include <Layout.hpp>
#include <QLibrary>

#include "../Logger.hpp"

struct LayoutLoader {
  static LayoutLoader  *p_instance;
private:
  BasicLogger          *p_logger;
  QLibrary              layoutLibrary;
  Func_CreateLayout     func_CreateLayout;
  Func_ReadLayout       func_ReadLayout;
  Func_ClearLayout      func_ClearLayout;
  Func_CopyLayout       func_CopyLayout;
  Func_WriteLayout      func_WriteLayout;
  Func_FreeLayout       func_FreeLayout;
private:
  LayoutLoader(BasicLogger *logger);
 ~LayoutLoader();
public:
  static void Init(BasicLogger *logger);
  static void Free();
  static LayoutLoader *GetInstance();
public:
  Layout *CreateLayoutFromFile(QString fileName);
  void ReadLayoutFromFile(QString fileName, Layout *layout);
  void ClearLayout(Layout *layout);
  void CopyLayout(Layout *src, Layout *dst);
  bool WriteLayout(Layout *layout, std::wstring fileName, FileFormat format);
  void FreeLayout(Layout **layout);
};

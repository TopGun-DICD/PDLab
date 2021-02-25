#pragma once

#include <Layout.hpp>

#include <QMainWindow>

class Helper {
  static Helper *p_instance;
private:
  QMainWindow *p_mainWindow;
private:
  Helper() : p_mainWindow(nullptr) {};
public:
  static void Init();
  static void Free();
  static Helper *GetInstance();
public:
  void AssignMainWindow(QMainWindow *window);
public:
  void ShowLayout(Layout *layout);
};
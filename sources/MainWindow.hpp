#pragma once

#include <QMainWindow>
#include <QAction>
#include <QStatusBar>
#include <QTextEdit>

#include "ConsoleWidget.hpp"
#include "FlowItemsListWidget.hpp"
#include "FlowWidget.hpp"
#include "Logger.hpp"
#include "UserFlowItemsManager.hpp"
#include "LayoutViewer/LayoutViewer.hpp"

class MainWindow : public QMainWindow {
  Q_OBJECT
private:
  Logger                   *p_logger;
  // Widgets
  ConsoleWidget            *p_console;
  FlowItemsListWidget      *p_itemsListWidget;
  UserFlowItemsListWidget  *p_userItemsListWidget;
  FlowWidget               *p_activeFlowWidget;
  UserFlowItemsManager     *p_userFlowsManager;
  LayoutViewer             *p_layoutViewer;
  // Dowck widgets
  QDockWidget              *p_dockFlowItems,
                           *p_dockConsole,
                           *p_dockLayout;
  // Actions
  QAction    *p_actFileNew,
             *p_actFileOpen,
             *p_actFileSave,
             *p_actFileSaveAs,
             *p_actFileExit,
             *p_actViewFlowItems,
             *p_actViewConsole,
             *p_actViewLayout,
             *p_actFlowRun,
             *p_actFlowRunTo,
             *p_actFlowStop,
             *p_actFlowReset,
             *p_actHelpAbout;
  // Status bar
  QStatusBar *p_statusBar;
public:
  MainWindow(Logger *logger);
 ~MainWindow();
private:
  void InitActions();
  void InitMenubar();
  void InitToolbar();
  void InitStatusbar();
  void InitMainUI();
private slots:
  void OnMenu_File_New();
  void OnMenu_File_Open();
  void OnMenu_File_Save();
  void OnMenu_File_SaveAs();
  void OnMenu_File_Exit();
  void OnMenu_Flow_Run();
  void OnMenu_Flow_RunTo();
  void OnMenu_Flow_Stop();
  void OnMenu_Flow_Reset();
  void OnMenu_Help_About();
public:
  void ShowLayout(Layout *layout);
};

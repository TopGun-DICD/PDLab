#include "MainWindow.hpp"

#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QTabWidget>
#include <QDockWidget>
#include <QIcon>
#include <QDateTime>

#include "Logger.hpp"
#include "FlowItems/FlowItem.hpp"
#include "Dialogs/About.hpp"
#include "Common.hpp"
#include "FlowItems/LayoutLoader.hpp"
#include "Helper.hpp"


MainWindow::MainWindow(Logger *logger) : QMainWindow(nullptr), p_logger(logger) {
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle(QString("Physical Design Lab - %1 [New Flow]").arg(VERSION_STRING));
  resize(QSize(1200, 800));

  InitMainUI();

  InitActions();
  InitMenubar();
  InitToolbar();
  InitStatusbar();

  Helper::Init();
  Helper::GetInstance()->AssignMainWindow(this);
}

MainWindow::~MainWindow() {
  Helper::Free();
  p_logger->AssignConsoleWidget(nullptr);
  delete p_userFlowsManager;
  p_userFlowsManager = nullptr;
}

void MainWindow::InitActions() {
  // Menu "File"
  p_actFileExit = new QAction(tr("Exit"), this);
  connect(p_actFileExit, SIGNAL(triggered()), SLOT(OnMenu_File_Exit()));

  // Menu "View"
  p_actViewFlowItems = p_dockFlowItems->toggleViewAction();
  p_actViewFlowItems->setIcon(QPixmap(":/toolbar/items.png"));
  p_actViewFlowItems->setText(tr("Flow items"));
  p_actViewFlowItems->setCheckable(true);
  p_actViewFlowItems->setChecked(true);

  p_actViewConsole = p_dockConsole->toggleViewAction();
  p_actViewConsole->setIcon(QPixmap(":/toolbar/console.png"));
  p_actViewConsole->setText(tr("Console"));
  p_actViewConsole->setCheckable(true);
  p_actViewConsole->setChecked(true);

  p_actViewLayout = p_dockLayout->toggleViewAction();
  p_actViewLayout->setIcon(QPixmap(":/toolbar/layout.png"));
  p_actViewLayout->setText(tr("Layout"));
  p_actViewLayout->setCheckable(true);
  p_actViewLayout->setChecked(false);

  // Menu "Flow"
  p_actFlowRun = new QAction(QPixmap(":/toolbar/run.png"), tr("Run Flow"), this);
  connect(p_actFlowRun, SIGNAL(triggered()), SLOT(OnMenu_Flow_Run()));

  p_actFlowRunTo = new QAction(QPixmap(":/toolbar/runto.png"), tr("Run Flow To"), this);
  connect(p_actFlowRunTo, SIGNAL(triggered()), SLOT(OnMenu_Flow_RunTo()));

  p_actFlowStop = new QAction(QPixmap(":/toolbar/stop.png"), tr("Stop Flow"), this);
  connect(p_actFlowStop, SIGNAL(triggered()), SLOT(OnMenu_Flow_Stop()));

  p_actFlowReset = new QAction(QPixmap(":/toolbar/reset.png"), tr("Reset Flow"), this);
  connect(p_actFlowReset, SIGNAL(triggered()), SLOT(OnMenu_Flow_Reset()));

  // Menu "Tools"

  // Menu "Help"
  p_actHelpAbout = new QAction(tr("About"), this);
  connect(p_actHelpAbout, SIGNAL(triggered()), SLOT(OnMenu_Help_About()));
}

void MainWindow::InitMenubar() {
  QMenu *p_menuFile   = new QMenu(tr("File"));
  QMenu *p_menuView   = new QMenu(tr("View"));
  QMenu *p_menuFlow   = new QMenu(tr("Flow"));
  QMenu *p_menuTools  = new QMenu(tr("Tools"));
  QMenu *p_menuHelp   = new QMenu(tr("Help"));

  p_menuFile->addAction(p_actFileExit);
  
  p_menuView->addAction(p_actViewFlowItems);
  p_menuView->addAction(p_actViewConsole);
  p_menuView->addSeparator();
  p_menuView->addAction(p_actViewLayout);

  p_menuFlow->addAction(p_actFlowRun);
  p_menuFlow->addAction(p_actFlowRunTo);
  p_menuFlow->addAction(p_actFlowStop);
  p_menuFlow->addAction(p_actFlowReset);

  p_menuHelp->addAction(p_actHelpAbout);

  menuBar()->addMenu(p_menuFile);
  menuBar()->addMenu(p_menuView);
  menuBar()->addMenu(p_menuFlow);
  menuBar()->addMenu(p_menuTools);
  menuBar()->addMenu(p_menuHelp);
}

void MainWindow::InitToolbar() {
  QToolBar* p_toolBar = new QToolBar(this);

  p_toolBar->addAction(p_actViewFlowItems);
  p_toolBar->addAction(p_actViewConsole);

  p_toolBar->addSeparator();

  p_toolBar->addAction(p_actFlowRun);
  p_toolBar->addAction(p_actFlowRunTo);
  p_toolBar->addAction(p_actFlowStop);
  p_toolBar->addAction(p_actFlowReset);

  p_toolBar->addSeparator();

  //QWidget *w = new QWidget;
  //w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  //p_toolBar->addWidget(w);

  p_toolBar->addAction(p_actViewLayout);

  addToolBar(Qt::TopToolBarArea, p_toolBar);
}

void MainWindow::InitStatusbar() {
  p_statusBar = new QStatusBar(this);
  setStatusBar(p_statusBar);
}

void MainWindow::InitMainUI() {
  // Console and Log
  p_dockConsole = new QDockWidget(tr("Console"), this);
  addDockWidget(Qt::BottomDockWidgetArea, p_dockConsole);
  p_dockConsole->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
  p_dockConsole->setAllowedAreas(Qt::BottomDockWidgetArea);

  p_console = new ConsoleWidget(p_dockConsole);
  p_logger->AssignConsoleWidget(p_console);
  p_dockConsole->setWidget(p_console);

  p_logger->EnableFileLogging(LogFormat::text);
  p_logger->Log(QString("--- Physical Design Lab %1 ---").arg(VERSION_STRING));
  QDateTime dt = QDateTime::currentDateTime();
  p_logger->Log(QString("Log started on %2 %1").arg(dt.time().toString()).arg(dt.date().toString()));

  p_userFlowsManager = new UserFlowItemsManager(p_logger);

  // FlowItems tree
  p_dockFlowItems = new QDockWidget(tr("Flow Items"), this);
  addDockWidget(Qt::LeftDockWidgetArea, p_dockFlowItems);
  p_dockFlowItems->setFixedWidth(270);
  p_dockFlowItems->setMinimumWidth(270);
  p_dockFlowItems->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
  p_dockFlowItems->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);

  QTabWidget *p_tabFlowItems = new QTabWidget(p_dockFlowItems);
  p_dockFlowItems->setWidget(p_tabFlowItems);
  p_tabFlowItems->setTabPosition(QTabWidget::South);
  p_itemsListWidget = new FlowItemsListWidget(p_dockFlowItems);
  p_tabFlowItems->addTab(p_itemsListWidget, QPixmap(":/pages/items.png"), tr("Standard items"));
  p_userItemsListWidget = new UserFlowItemsListWidget(p_dockFlowItems, p_userFlowsManager);
  p_tabFlowItems->addTab(p_userItemsListWidget, tr("User items"));

  // Layout Viewer
  p_dockLayout = new QDockWidget(tr("Layout Viewer"), this);
  addDockWidget(Qt::RightDockWidgetArea, p_dockLayout);
  p_dockLayout->setMinimumWidth(400);
  //p_dockLayout->resize(400, 300);
  p_dockLayout->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
  p_dockLayout->setAllowedAreas(Qt::RightDockWidgetArea);
  
  p_layoutViewer = new LayoutViewer(p_dockLayout);
  p_dockLayout->setWidget(p_layoutViewer);
  p_dockLayout->hide();

  // Central tab widget
  QTabWidget* p_tabFlows = new QTabWidget(this);
  p_activeFlowWidget = new FlowWidget(this, p_logger, p_userFlowsManager);
  p_tabFlows->addTab(p_activeFlowWidget, QPixmap(":/pages/flow.png"), tr("New flow"));
  setCentralWidget(p_tabFlows);
}

void MainWindow::OnMenu_File_Exit() {
  close();
}

void MainWindow::OnMenu_Flow_Run() {
}

void MainWindow::OnMenu_Flow_RunTo() {
}

void MainWindow::OnMenu_Flow_Stop() {
}

void MainWindow::OnMenu_Flow_Reset() {
  foreach(QGraphicsItem * p_item, p_activeFlowWidget->items()) {
    // Skip port and paths
    if (p_item->type() != QGraphicsItem::UserType + 1)
      continue;
    FlowItem *p_flowItem = static_cast<FlowItem *>(p_item);
    if (p_flowItem->GetStatus() == FlowItemStatus::completed)
      p_flowItem->OnHandleEvent_Reset();
    p_item->update();
  }

  p_dockLayout->hide();
  p_layoutViewer->AssignLayout(nullptr);
  p_layoutViewer->update();
}

void MainWindow::OnMenu_Help_About() {
  Dlg_About dlg(this);
  dlg.exec();
}

void MainWindow::ShowLayout(Layout *layout) {
  if (!layout) {
    p_logger->Warning("Was asked to open layout viewer but layout is nullptr. Operation ignored.");
    return;
  }
  p_layoutViewer->AssignLayout(layout);
  if (p_dockLayout->isHidden())
    p_dockLayout->show();
  
  p_layoutViewer->update();
}


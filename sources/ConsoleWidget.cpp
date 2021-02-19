#include "ConsoleWidget.hpp"

#include <QLayout>
#include <QTabWidget>
#include <QLabel>

#include "Logger.hpp"

ConsoleWidget::ConsoleWidget(QWidget* parent) : QWidget(parent) {
  QHBoxLayout* p_hLayout = new QHBoxLayout;
  p_hLayout->setSpacing(5);
  p_hLayout->setMargin(1);

  QTabWidget* p_tabConsole = new QTabWidget(this);
  p_tabConsole->setTabPosition(QTabWidget::South);
  p_console = new QTextEdit(p_tabConsole);
  p_tabConsole->addTab(p_console, QPixmap(":/pages/console.png"), tr("Log"));
  p_luaConsole = new LuaCodeEditor(false, p_tabConsole);
  p_tabConsole->addTab(p_luaConsole, QPixmap(":/pages/lua.png"), tr("Lua console"));
  p_hLayout->addWidget(p_tabConsole, 1);

  QVBoxLayout* p_vLayout = new QVBoxLayout;
  p_vLayout->addWidget(new QLabel("Item info:"));
  p_itemInfo = new QTextEdit(this);
  p_itemInfo->resize(QSize(500, 100));
  p_itemInfo->setEnabled(false);
  p_vLayout->addWidget(p_itemInfo, 1);

  p_hLayout->addLayout(p_vLayout);

  setLayout(p_hLayout);
}

void ConsoleWidget::Echo(const QString& text) {
  p_console->append(text);
}

void ConsoleWidget::Info(QString text) {
  p_itemInfo->setText(text);
}

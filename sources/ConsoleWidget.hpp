#pragma once

#include <QWidget>
#include <QTextEdit>

#include "LuaCodeEditor.hpp"
#include "LogTextWidget.hpp"

class ConsoleWidget : public QWidget {
  QTextEdit      *p_itemInfo;
  LogTextWidget  *p_console;
  LuaCodeEditor  *p_luaConsole;
public:
  ConsoleWidget(QWidget *parent);
public:
  void Echo(const QString& text);
  void Info(QString text);
};

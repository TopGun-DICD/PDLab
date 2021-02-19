#pragma once

#include <QWidget>
#include <QTextEdit>

#include "LuaCodeEditor.hpp"

class ConsoleWidget : public QWidget {
  QTextEdit      *p_console;
  QTextEdit      *p_itemInfo;
  LuaCodeEditor  *p_luaConsole;
public:
  ConsoleWidget(QWidget *parent);
public:
  void Echo(const QString& text);
  void Info(QString text);
};

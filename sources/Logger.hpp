#pragma once

#include <QString>

#include "ConsoleWidget.hpp"
#include "Types.hpp"

class BasicLogger {
public:
  virtual void Log(const QString &text) = 0;
  virtual void Warning(const QString &text) = 0;
  virtual void Error(const QString &text) = 0;
};

class Logger : public BasicLogger {
  LogFormat       logFormat;
  ConsoleWidget  *p_console;
public:
  Logger();
 ~Logger();
public:
  void AssignConsoleWidget(ConsoleWidget *console);
  void EnableFileLogging(LogFormat format = LogFormat::none);
public:
  void ShowInfoString(QString text);
public:
  void Log(const QString &text) override final;
  void Warning(const QString& text) override final;
  void Error(const QString& text) override final;
};

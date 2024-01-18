#include "Logger.hpp"

#include <QTextStream>

Logger::Logger() : logFormat(LogFormat::none) {}

Logger::~Logger() {}

void Logger::AssignConsoleWidget(ConsoleWidget* console) {
  p_console = console;
}

void Logger::EnableFileLogging(LogFormat format) {
  if (format == LogFormat::none)
    return;

  logFormat = format;

  QString fileName;

  switch (logFormat) {
  case LogFormat::text:
    fileName = "PDL_log.log";
    break;
  case LogFormat::html:
    fileName = "PDL_log.html";
    break;
  case LogFormat::rtf:
    fileName = "PDL_log.rtf";
    break;
  }

  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;
  QTextStream stream(&file);

  file.close();
}

void Logger::ShowInfoString(QString text) {
  if (!p_console)
    return;
  p_console->Info(text);
}

void Logger::Log(const QString& text) {
  if (!p_console)
    return;
  p_console->Echo(QString("%1").arg(text));

  if (logFormat == LogFormat::none)
    return;

  QString fileName;

  switch (logFormat) {
    case LogFormat::text:
      fileName = "PDL_log.log";
      break;
    case LogFormat::html:
      fileName = "PDL_log.html";
      break;
    case LogFormat::rtf:
      fileName = "PDL_log.rtf";
      break;
  }

  QFile file(fileName);
  if (!file.open(QIODevice::Append | QIODevice::Text))
    return;
  QTextStream stream(&file);
  
  switch (logFormat) {
    case LogFormat::text:
      stream << text << "\n";
      break;
  }
  
  file.close();
}

void Logger::Warning(const QString& text) {
  if (!p_console)
    return;
  p_console->Echo(QString("[warning] : %1").arg(text));

  if (logFormat == LogFormat::none)
    return;

  QString fileName;

  switch (logFormat) {
  case LogFormat::text:
    fileName = "PDL_log.log";
    break;
  case LogFormat::html:
    fileName = "PDL_log.html";
    break;
  case LogFormat::rtf:
    fileName = "PDL_log.rtf";
    break;
  }

  QFile file(fileName);
  if (!file.open(QIODevice::Append | QIODevice::Text))
    return;
  QTextStream stream(&file);

  switch (logFormat) {
  case LogFormat::text:
    stream << "[warning] : " << text << "\n";
    break;
  }

  file.close();
}

void Logger::Error(const QString& text) {
  if (!p_console)
    return;
  p_console->Echo(QString("[error] : %1").arg(text));

  if (logFormat == LogFormat::none)
    return;

  QString fileName;

  switch (logFormat) {
  case LogFormat::text:
    fileName = "PDL_log.log";
    break;
  case LogFormat::html:
    fileName = "PDL_log.html";
    break;
  case LogFormat::rtf:
    fileName = "PDL_log.rtf";
    break;
  }

  QFile file(fileName);
  if (!file.open(QIODevice::Append | QIODevice::Text))
    return;
  QTextStream stream(&file);

  switch (logFormat) {
  case LogFormat::text:
    stream << "[error] : " << text << "\n";
    break;
  }

  file.close();
}


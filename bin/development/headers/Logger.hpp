#pragma once

#include <QString>

#include "Types.hpp"

class BasicLogger {
public:
  virtual void Log(const QString &text);
  virtual void Warning(const QString &text);
  virtual void Error(const QString &text);
};


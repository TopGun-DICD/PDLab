#pragma once

enum class FlowItemType {
  unknown = 0,
  // I/O
  importdata,
  editdata,
  exportdata,
  // Layer operations
  extract,
  // User flow items
  userdefined,
};

enum class FlowItemStatus {
  undefined = 0,
  unknown,
  inprogress,
  completed,
};

enum class LogFormat {
  none = 0,
  text,
  html,
  rtf,
};

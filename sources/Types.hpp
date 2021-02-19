#pragma once

enum class FlowItemType {
  undefined = 0,
  // User flow items
  userdefined,
  // I/O
  importdata,
  editdata,
  exportdata,
  // Layer operations
  map,
  extract,
  // Layout operations
  heal,
};

enum class FlowItemStatus {
  undefined = 0,
  unknown,
  inprogress,
  completed,
};

enum class LogFormat {
  undefined = 0,
  none,
  text,
  html,
  rtf,
};

enum class LayoutOwnershipMode {
  undefined = 0,
  make_new,
  make_copy,
  make_link,
  make_nothing,
};

enum class PortDirection {
  undefnied = 0,
  input,
  output,
};

enum class PortDataType {
  undefined = 0,
  layout,
  lef,
  def,
  verilog
};
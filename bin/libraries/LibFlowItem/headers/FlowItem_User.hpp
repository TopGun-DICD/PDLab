#pragma once

#include "FlowItem.hpp"

class FlowItem_User : public FlowItem {
public:
  FlowItem_User(QString title, BasicLogger *logger);
  virtual ~FlowItem_User();
public:
  virtual bool OnHandleEvent_Drop() = 0;
  virtual bool OnHandleEvent_Execute() = 0;
  virtual bool OnHandleEvent_Reset() = 0;
  virtual bool OnHandleEvent_Properties() = 0;
  QString GetItemTypeAsString() { return "User-Defined"; };
};
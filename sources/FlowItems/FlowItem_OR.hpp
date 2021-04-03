#include "FlowItem.hpp"

class FlowItem_OR : public FlowItem {
public:
  FlowItem_OR(BasicLogger *logger);
  virtual ~FlowItem_OR();
public:
  virtual bool DropEventHandler() override final;
  virtual bool ExecuteEventHandler()override final;
  virtual bool OpenResultsEventHandler() override final;
  virtual bool ResetEventHandler() override final;
  virtual bool ShowPropertesEventHandler() override final;
  QString GetInfoString() override final;
  QString GetItemTypeAsString() override final { return "OR"; };
};

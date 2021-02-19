#include "FlowItem.hpp"

class FlowItem_Map : public FlowItem {
public:
  FlowItem_Map(BasicLogger *logger);
  virtual ~FlowItem_Map();
public:
  virtual bool DropEventHandler() override final;
  virtual bool ExecuteEventHandler()override final;
  virtual bool OpenResultsEventHandler() override final;
  virtual bool ResetEventHandler() override final;
  virtual bool ShowPropertesEventHandler() override final;
  QString GetInfoString() override final;
  QString GetItemTypeAsString() override final { return "Edit"; };
};

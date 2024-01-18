#include "FlowItem.hpp"

class FlowItem_AND : public FlowItem {
public:
  FlowItem_AND(BasicLogger *logger);
  virtual ~FlowItem_AND();
public:
  virtual bool DropEventHandler() override final;
  virtual bool ExecuteEventHandler()override final;
  virtual bool OpenResultsEventHandler() override final;
  virtual bool ResetEventHandler() override final;
  virtual bool ShowPropertiesEventHandler() override final;
  QString GetInfoString() override final;
  QString GetItemTypeAsString() override final { return "AND"; };
};

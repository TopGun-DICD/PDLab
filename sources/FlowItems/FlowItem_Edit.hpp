#include "FlowItem.hpp"

class FlowItem_Edit : public FlowItem {
  QString tempLayoutFileName;
public:
  FlowItem_Edit(BasicLogger *logger);
  virtual ~FlowItem_Edit();
public:
  virtual bool DropEventHandler() override final;
  virtual bool ExecuteEventHandler()override final;
  virtual bool OpenResultsEventHandler() override final;
  virtual bool ResetEventHandler() override final;
  virtual bool ShowPropertiesEventHandler() override final;
  QString GetInfoString() override final;
  QString GetItemTypeAsString() override final { return "Extract"; };
};

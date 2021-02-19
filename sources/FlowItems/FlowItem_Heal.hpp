#include "FlowItem.hpp"

class FlowItem_Heal : public FlowItem {
public:
  FlowItem_Heal(BasicLogger *logger);
  virtual ~FlowItem_Heal();
public:
  virtual bool DropEventHandler() override final;
  virtual bool ExecuteEventHandler()override final;
  virtual bool OpenResultsEventHandler() override final;
  virtual bool ResetEventHandler() override final;
  virtual bool ShowPropertesEventHandler() override final;
  QString GetInfoString() override final;
  QString GetItemTypeAsString() override final { return "Edit"; };
};

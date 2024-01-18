#include "FlowItem.hpp"

#include "../Dialogs/Extract.hpp"

class FlowItem_Extract : public FlowItem {
  LayersInfo  layersInfo;
public:
  FlowItem_Extract(BasicLogger *logger);
  virtual ~FlowItem_Extract();
public:
  virtual bool DropEventHandler() override final;
  virtual bool ExecuteEventHandler()override final;
  virtual bool OpenResultsEventHandler() override final;
  virtual bool ResetEventHandler() override final;
  virtual bool ShowPropertiesEventHandler() override final;
  QString GetInfoString() override final;
  QString GetItemTypeAsString() override final { return "Edit"; };
};

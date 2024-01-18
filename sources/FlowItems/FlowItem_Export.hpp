#include "FlowItem.hpp"

class FlowItem_Export : public FlowItem {
  QString         fileName,
                  fileShortName,
                  techFileName;
  __int64         fileSize;
public:
  FlowItem_Export(BasicLogger *logger);
  virtual ~FlowItem_Export();
public:
  virtual bool DropEventHandler() override final;
  virtual bool ExecuteEventHandler()override final;
  virtual bool OpenResultsEventHandler() override final;
  virtual bool ResetEventHandler() override final;
  virtual bool ShowPropertiesEventHandler() override final;
  QString GetInfoString() override final;
  QString GetItemTypeAsString() override final { return "Export"; };
};

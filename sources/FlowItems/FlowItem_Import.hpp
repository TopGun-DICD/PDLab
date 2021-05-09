#include "FlowItem.hpp"

class FlowItem_Import : public FlowItem {
  QString         fileName,
                  fileShortName,
                  techFileName;
  __int64         fileSize;
  QStringList     layersToExtract;
public:
  FlowItem_Import(BasicLogger *logger);
  virtual ~FlowItem_Import();
public:
  virtual bool DropEventHandler() override final;
  virtual bool ExecuteEventHandler()override final;
  virtual bool OpenResultsEventHandler() override final;
  virtual bool ResetEventHandler() override final;
  virtual bool ShowPropertesEventHandler() override final;
  QString GetInfoString() override final;
  QString GetItemTypeAsString() override final { return "Import"; };
};

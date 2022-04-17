#include "FlowItem.hpp"

#include "../Dialogs/Import.hpp"

class FlowItem_Import : public FlowItem {
  QString         fileName,
                  fileShortName,
                  techFileName,
                  formatInfoString,
                  selectedLayers,
                  ignoredLayers;
  int64_t         fileSize;
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
private:
  bool InitDataFromDialogCall(const Dlg_Import &dlg);
  bool ParseLayersString(const QString& s, std::vector<int>& v);
  void ApplyTechFileData();
};

#include "FlowItem.hpp"

class FlowItem_User : public FlowItem {
public:
  FlowItem_User(QString title, BasicLogger *logger, LayoutOwnershipMode mode);
  virtual ~FlowItem_User();
public:
  QString GetItemTypeAsString() override final { return "User-Defined"; }
public:
  virtual bool DropEventHandler() = 0;
  virtual bool ExecuteEventHandler() = 0;
  virtual bool OpenResultsEventHandler() = 0;
  virtual bool ResetEventHandler() = 0;
  virtual bool ShowPropertiesEventHandler() = 0;

  virtual QString GetInfoString() = 0;
};

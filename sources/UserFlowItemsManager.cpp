#include "UserFlowItemsManager.hpp"

#include <QDir>


#include "FlowItems/FlowItem.hpp"

UserFlowItemInfo::UserFlowItemInfo(BasicLogger *logger) : p_item(nullptr), p_logger(logger), func_GetItem(nullptr), func_FreeItem(nullptr) {}
UserFlowItemInfo::~UserFlowItemInfo() {
  if (library.isLoaded()) {
    if (p_item && func_FreeItem) {
      func_FreeItem(&p_item);
      library.unload();
    }
  }
}

bool UserFlowItemInfo::LoadFromLibrary(QString fName) {
  fileName = fName;
  library.setFileName(fileName);
  if (!library.load()) {
    p_logger->Error(QString("\tCan't load library %1.\n\tLibrary will be ignored.").arg(fileName));
    return false;
  }
  if (!library.isLoaded()) {
    p_logger->Error(QString("\tCan't load library %1.\n\tLibrary will be ignored.").arg(fileName));
    return false;
  }
  // Check DLL_GetItemType function
  Func_GetType func_Type = (Func_GetType)library.resolve("GetType");
  if (func_Type) {
    if (func_Type() != FlowItemType::userdefined) {
      p_logger->Error("\tLibrary has type different from 'User-Defined'.\n\tLibrary will be ignored.");
      library.unload();
      return false;
    }
  }
  else {
    p_logger->Error(QString("\tLibrary doesn't have one or more required functions [function 'GetType' was not found].\n\tLibrary will be ignored."));
    library.unload();
    return false;
  }

  // Check DLL_GetFlowItemInfoFunc function
  Func_GetInfo func_Info = (Func_GetInfo)library.resolve("GetInfo");
  if (!func_Info) {
    p_logger->Error(QString("\tLibrary doesn't have one or more required functions [function 'GetInfo' was not found].\n\tLibrary will be ignored."));
    library.unload();
    return false;
  }
  flowItemInfo = func_Info();

  // Check DLL_GetFlowItemCaptionFunc function
  Func_GetCaption func_Caption = (Func_GetCaption)library.resolve("GetCaption");
  if (!func_Caption) {
    p_logger->Error(QString("\tLibrary doesn't have one or more required functions [function 'GetCaption' was not found].\n\tLibrary will be ignored."));
    library.unload();
    return false;
  }
  caption = func_Caption();

  // Check DLL_GetAuthorInfoFunc function
  Func_GetAuthor func_Author = (Func_GetAuthor)library.resolve("GetAuthor");
  if (!func_Author) {
    p_logger->Error(QString("\tLibrary doesn't have one or more required functions [function 'GetAuthor' was not found].\n\tLibrary will be ignored."));
    library.unload();
    return false;
  }
  authorInfo = func_Author();

  // Check DLL_GetFlowItemFunc function
  func_GetItem = (Func_GetItem)library.resolve("GetItem");
  if (!func_GetItem) {
    p_logger->Error(QString("\tLibrary doesn't have one or more required functions [function 'GetItem' was not found].\n\tLibrary will be ignored."));
    library.unload();
    return false;
  }

  // Check DLL_FreeFlowItemFunc function
  func_FreeItem = (Func_FreeItem)library.resolve("FreeItem");
  if (!func_FreeItem) {
    p_logger->Error(QString("\tLibrary doesn't have one or more required functions [function 'FreeItem' was not found].\n\tLibrary will be ignored."));
    library.unload();
    return false;
  }
  return true;
}

FlowItem *UserFlowItemInfo::GetFlowItem() {
  if (!library.isLoaded())
    return nullptr;
  if (!func_GetItem)
    return nullptr;
  return func_GetItem(p_logger);
}

UserFlowItemsManager::UserFlowItemsManager(BasicLogger *logger) {
#if defined _DEBUG
  QDir directory("modulesd");
#else
  QDir directory("modules");
#endif
  QStringList dllnames = directory.entryList(QStringList() << "*.dll", QDir::Files);
  int totalDlls = 0;
  
  logger->Log(QString("\nStarting 'modules' directory files processing (found %1 libraries in 'modules' directory.)...").arg(dllnames.length()));

  foreach(QString dllname, dllnames) {
    ++totalDlls;

    logger->Log(QString("Processing library '%1' [%2 of %3]...").arg(dllname).arg(totalDlls).arg(dllnames.length()));
    
    UserFlowItemInfo *p_ufii = new UserFlowItemInfo(logger);
    if (!p_ufii->LoadFromLibrary(directory.dirName() +  "/" + dllname)) {
      delete p_ufii;
      p_ufii = nullptr;
      continue;
    }

    userFlowItems.push_back(p_ufii);
  }

  logger->Log(QString("Files in 'modules' directory were processed. %1 file(s) found in 'modules' directory, %2 file(s) accepted.\n").arg(totalDlls).arg(userFlowItems.size()));
}

UserFlowItemsManager::~UserFlowItemsManager() {
  foreach(UserFlowItemInfo *itemInfo, userFlowItems) {
    delete itemInfo;
    itemInfo = nullptr;
  }
}

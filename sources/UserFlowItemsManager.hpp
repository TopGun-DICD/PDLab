#pragma once

#include <QString>
#include <QLibrary>

#include "Types.hpp"
#include "Logger.hpp"
#include "FlowItems/FlowItem.hpp"

typedef FlowItemType  (* Func_GetType)();
typedef QString       (* Func_GetCaption)();
typedef QString       (* Func_GetInfo)();
typedef QString       (* Func_GetAuthor)();
typedef FlowItem *    (* Func_GetItem)(BasicLogger *);
typedef void          (* Func_FreeItem)(FlowItem **);

class UserFlowItemInfo {
public:
  QString         fileName,
                  caption,
                  flowItemInfo,
                  authorInfo;
  FlowItem       *p_item;
  BasicLogger    *p_logger;
  QLibrary        library;
private:
  Func_GetItem    func_GetItem;
  Func_FreeItem   func_FreeItem;
public:
  UserFlowItemInfo(BasicLogger *logger);
 ~UserFlowItemInfo();
public:
  bool LoadFromLibrary(QString fName);
  FlowItem *GetFlowItem();
};

class UserFlowItemsManager {
public:
  QVector<UserFlowItemInfo *> userFlowItems;
public:
  UserFlowItemsManager(BasicLogger *logger);
 ~UserFlowItemsManager();
};

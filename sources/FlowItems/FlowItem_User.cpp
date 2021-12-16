#include "FlowItem_User.hpp"

#include "../Config.hpp"

FlowItem_User::FlowItem_User(QString title, BasicLogger *logger, LayoutOwnershipMode mode) : FlowItem(FlowItemType::userdefined, title, logger, mode) {
  titleBgColor = Config::Get()->colors.headerUserDefined;
}

FlowItem_User::~FlowItem_User() {
}

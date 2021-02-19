#include "FlowItem_User.hpp"

FlowItem_User::FlowItem_User(QString title, BasicLogger *logger, LayoutOwnershipMode mode) : FlowItem(FlowItemType::userdefined, title, logger, mode) {
  titleBgColor = QColor(38, 38, 38);
}

FlowItem_User::~FlowItem_User() {
}

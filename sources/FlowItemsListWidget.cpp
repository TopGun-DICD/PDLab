#include "FlowItemsListWidget.hpp"

#include <QScrollBar>
#include <QMimeData>
#include <QDrag>
#include <QPainter>

#include "Config.hpp"

CategoryButton::CategoryButton(const QString& text, QWidget* parent, QTreeWidgetItem* item) : QPushButton(text, parent), p_item(item) {
  connect(this, SIGNAL(pressed()), this, SLOT(ButtonPressed()));
}

CustomListItem::CustomListItem(FlowItemType it, const QPixmap &pixmap, const QString &text, int id) : QWidget(nullptr), type(it), uniqueID(id) {
  icon = pixmap;
}

QPixmap& CustomListItem::GetPixmap() {
  return icon;
}

FlowItemType CustomListItem::GetType() {
  return type;
}

int CustomListItem::GetUniueID() {
  return uniqueID;
}

void CategoryButton::ButtonPressed() {
  p_item->setExpanded(!p_item->isExpanded());
}

/***** FlowItems List Widget *****/

FlowItemsListWidget::FlowItemsListWidget(QWidget *parent) : QTreeWidget(parent) {
  setHeaderHidden(true);
  setRootIsDecorated(false);
  setIndentation(0);
  //setVerticalScrollMode()
  
  QPixmap px(QSize(100, 25));
  QPainter painter(&px);
  QFontMetrics fm(painter.font());

  FillSection_LayoutIO(painter, px, fm);
  FillSection_LayerOperations(painter, px, fm);
  FillSection_LayoutOperations(painter, px, fm);

  painter.end();

  expandAll();
}

void FlowItemsListWidget::FillSection_LayoutIO(QPainter &painter, QPixmap &px, QFontMetrics &fm) {
  QListWidgetItem *p_item = nullptr;
  painter.setPen(QColor(255, 255, 255, 255));
  int tw = 0;
  int th = fm.height();

  // Category : I/O
  QTreeWidgetItem *p_catIO = new QTreeWidgetItem;
  CategoryButton* p_btnIO = new CategoryButton(tr("Layout Import/Export"), this, p_catIO);
  addTopLevelItem(p_catIO);
  setItemWidget(p_catIO, 0, p_btnIO);

  ItemsListWidget* p_lstIO = new ItemsListWidget;
  p_lstIO->show();
  p_lstIO->setFixedHeight(66);
  p_lstIO->setFrameShape(QFrame::Shape::NoFrame);

  p_lstIO->setViewMode(QListWidget::IconMode);
  p_lstIO->setIconSize(QSize(100, 25));
  p_lstIO->setResizeMode(QListView::Adjust);
  p_lstIO->verticalScrollBar()->setDisabled(true);
  p_lstIO->verticalScrollBar()->setVisible(false);
  p_lstIO->setAcceptDrops(false);

  // I/O : Import
  p_item = new QListWidgetItem;
  px.fill(Config::Get()->colors.headerImportExport);
  tw = fm.horizontalAdvance(tr("IMPORT"));
  painter.drawText(50 - tw / 2, 10 + th / 2, tr("IMPORT"));
  p_item->setIcon(px);
  p_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
  p_lstIO->addItem(p_item);
  p_lstIO->setItemWidget(p_item, new CustomListItem(FlowItemType::importdata, px, tr("Import")));
  // End of I/O : Import

  // I/O : Edit
  //*
  p_item = new QListWidgetItem;
  px.fill(Config::Get()->colors.headerImportExport);
  tw = fm.horizontalAdvance(tr("EDIT"));
  painter.drawText(50 - tw / 2, 10 + th / 2, tr("EDIT"));
  p_item->setIcon(px);
  p_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
  p_lstIO->addItem(p_item);
  p_lstIO->setItemWidget(p_item, new CustomListItem(FlowItemType::editdata, px, tr("Edit")));
  //*/
  // End of I/O : Edit

  // I/O : Export
  p_item = new QListWidgetItem;
  px.fill(Config::Get()->colors.headerImportExport);
  tw = fm.horizontalAdvance(tr("EXPORT"));
  painter.drawText(50 - tw / 2, 10 + th / 2, tr("EXPORT"));
  p_item->setIcon(px);
  p_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
  p_lstIO->addItem(p_item);
  p_lstIO->setItemWidget(p_item, new CustomListItem(FlowItemType::exportdata, px, tr("Export")));
  // End of I/O : Export

  QTreeWidgetItem *p_itemIOOperations = new QTreeWidgetItem;
  p_itemIOOperations->setDisabled(true);
  p_catIO->addChild(p_itemIOOperations);
  setItemWidget(p_itemIOOperations, 0, p_lstIO);
}

void FlowItemsListWidget::FillSection_LayerOperations(QPainter &painter, QPixmap &px, QFontMetrics &fm) {
  QListWidgetItem *p_item = nullptr;
  painter.setPen(QColor(255, 255, 255, 255));
  int tw = 0;
  int th = fm.height();

  // Category : Layer Operations
  QTreeWidgetItem *p_catLayersOps = new QTreeWidgetItem;
  CategoryButton* p_btnLayersOps = new CategoryButton(tr("Layer Operations"), this, p_catLayersOps);
  addTopLevelItem(p_catLayersOps);
  setItemWidget(p_catLayersOps, 0, p_btnLayersOps);

  ItemsListWidget* p_lstLayersOps = new ItemsListWidget;
  p_lstLayersOps->show();
  p_lstLayersOps->setFixedHeight(33);
  p_lstLayersOps->setFrameShape(QFrame::Shape::NoFrame);

  p_lstLayersOps->setViewMode(QListWidget::IconMode);
  p_lstLayersOps->setIconSize(QSize(100, 25));
  p_lstLayersOps->setResizeMode(QListView::Adjust);
  p_lstLayersOps->verticalScrollBar()->setDisabled(true);
  p_lstLayersOps->verticalScrollBar()->setVisible(false);
  p_lstLayersOps->setAcceptDrops(false);

  // Layer Operations : Map
  p_item = new QListWidgetItem;
  px.fill(Config::Get()->colors.headerLayerOperations);
  tw = fm.horizontalAdvance(tr("MAP"));
  painter.drawText(50 - tw / 2, 10 + th / 2, tr("MAP"));
  p_item->setIcon(px);
  p_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
  p_lstLayersOps->addItem(p_item);
  p_lstLayersOps->setItemWidget(p_item, new CustomListItem(FlowItemType::map, px, tr("Map")));
  // End of Layer Operations : Map

  // Layer Operations : Extract
  p_item = new QListWidgetItem;
  px.fill(Config::Get()->colors.headerLayerOperations);
  tw = fm.horizontalAdvance(tr("EXTRACT"));
  painter.drawText(50 - tw / 2, 10 + th / 2, tr("EXTRACT"));
  p_item->setIcon(px);
  p_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
  p_lstLayersOps->addItem(p_item);
  p_lstLayersOps->setItemWidget(p_item, new CustomListItem(FlowItemType::extract, px, tr("Extract")));
  // End of Layer Operations : Extract

  QTreeWidgetItem *p_itemLayerOperations = new QTreeWidgetItem;
  p_itemLayerOperations->setDisabled(true);
  p_catLayersOps->addChild(p_itemLayerOperations);
  setItemWidget(p_itemLayerOperations, 0, p_lstLayersOps);
}

void FlowItemsListWidget::FillSection_LayoutOperations(QPainter &painter, QPixmap &px, QFontMetrics &fm) {
  QListWidgetItem *p_item = nullptr;
  painter.setPen(QColor(255, 255, 255, 255));
  int tw = 0;
  int th = fm.height();

  // Category : Layout Operations
  QTreeWidgetItem *p_catLayoutOps = new QTreeWidgetItem;
  CategoryButton* p_btnLayoutOps = new CategoryButton(tr("Layout Operations"), this, p_catLayoutOps);
  addTopLevelItem(p_catLayoutOps);
  setItemWidget(p_catLayoutOps, 0, p_btnLayoutOps);

  ItemsListWidget* p_lstLayoutOps = new ItemsListWidget;
  p_lstLayoutOps->show();
  p_lstLayoutOps->setFixedHeight(33);
  p_lstLayoutOps->setFrameShape(QFrame::Shape::NoFrame);

  p_lstLayoutOps->setViewMode(QListWidget::IconMode);
  p_lstLayoutOps->setIconSize(QSize(100, 25));
  p_lstLayoutOps->setResizeMode(QListView::Adjust);
  p_lstLayoutOps->verticalScrollBar()->setDisabled(true);
  p_lstLayoutOps->verticalScrollBar()->setVisible(false);
  p_lstLayoutOps->setAcceptDrops(false);

  // Layout Operations : AND
  /*
  p_item = new QListWidgetItem;
  px.fill(QColor(139, 55, 78));
  tw = fm.horizontalAdvance(tr("AND"));
  painter.drawText(50 - tw / 2, 10 + th / 2, tr("AND"));
  p_item->setIcon(px);
  p_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
  p_lstLayoutOps->addItem(p_item);
  p_lstLayoutOps->setItemWidget(p_item, new CustomListItem(FlowItemType::AND, px, tr("AND")));
  */
  // End of Layout Operations : AND

  // Layout Operations : OR
  p_item = new QListWidgetItem;
  px.fill(Config::Get()->colors.headerLayoutOperations);
  tw = fm.horizontalAdvance(tr("OR"));
  painter.drawText(50 - tw / 2, 10 + th / 2, tr("OR"));
  p_item->setIcon(px);
  p_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
  p_lstLayoutOps->addItem(p_item);
  p_lstLayoutOps->setItemWidget(p_item, new CustomListItem(FlowItemType::OR, px, tr("OR")));
  // End of Layout Operations : OR

  // Layout Operations : Heal
  /*
  p_item = new QListWidgetItem;
  px.fill(QColor(139, 55, 78));
  tw = fm.horizontalAdvance(tr("HEAL"));
  painter.drawText(50 - tw / 2, 10 + th / 2, tr("HEAL"));
  p_item->setIcon(px);
  p_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
  p_lstLayoutOps->addItem(p_item);
  p_lstLayoutOps->setItemWidget(p_item, new CustomListItem(FlowItemType::heal, px, tr("Heal")));
  */
  // End of Layout Operations : Heal

  QTreeWidgetItem *p_itemLayoutOperations = new QTreeWidgetItem;
  p_itemLayoutOperations->setDisabled(true);
  p_catLayoutOps->addChild(p_itemLayoutOperations);
  setItemWidget(p_itemLayoutOperations, 0, p_lstLayoutOps);

}

/***** Items List Widget *****/

ItemsListWidget::ItemsListWidget(QWidget *parent) : QListWidget(parent) {}

void ItemsListWidget::startDrag(Qt::DropActions supportedActions) {
  if (!(supportedActions & Qt::CopyAction)) {
    QListWidget::startDrag(supportedActions);
    return;
  }

  QListWidgetItem *p_wgt = currentItem();
  if (!p_wgt)
    return;

  CustomListItem* p_item = static_cast<CustomListItem *>(itemWidget(p_wgt));
  QPixmap px = p_item->GetPixmap();
  QByteArray itemData;
  QDataStream dataStream(&itemData, QIODevice::WriteOnly);
  dataStream << p_item->GetType() << p_item->GetUniueID();

  QMimeData *mimeData = new QMimeData;
  mimeData->setData("application/x-dndflowitemdata", itemData);

  QDrag* drag = new QDrag(this);
  drag->setPixmap(px);
  drag->setMimeData(mimeData);
  drag->setHotSpot(px.rect().center());
  drag->exec(Qt::CopyAction);
}



/***** User FlowItems List Widget *****/

UserFlowItemsListWidget::UserFlowItemsListWidget(QWidget *parent, UserFlowItemsManager *manager) : QTreeWidget(parent) {
  setHeaderHidden(true);
  setRootIsDecorated(false);
  setIndentation(0);

  QListWidgetItem *p_item = nullptr;
  QPixmap px(QSize(100, 25));
  QPainter painter(&px);
  painter.setPen(QColor(255, 255, 255, 255));
  QFontMetrics fm(painter.font());

  int tw = 0;
  int th = fm.height();

  
  // Category : I/O
  QTreeWidgetItem *p_catIO = new QTreeWidgetItem;
  CategoryButton* p_btnIO = new CategoryButton(tr("User Items"), this, p_catIO);
  addTopLevelItem(p_catIO);
  setItemWidget(p_catIO, 0, p_btnIO);

  UserItemsListWidget* p_lstIO = new UserItemsListWidget;
  p_lstIO->show();
  //p_lstIO->setFixedHeight(65);
  p_lstIO->setFrameShape(QFrame::Shape::NoFrame);

  p_lstIO->setViewMode(QListWidget::IconMode);
  p_lstIO->setIconSize(QSize(100, 25));
  p_lstIO->setResizeMode(QListView::Adjust);
  p_lstIO->verticalScrollBar()->setDisabled(true);
  p_lstIO->verticalScrollBar()->setVisible(false);
  p_lstIO->setAcceptDrops(false);

  for (int i = 0; i < manager->userFlowItems.size(); ++i) {
    // I/O : Import
    p_item = new QListWidgetItem;
    px.fill(QColor(38, 38, 38));
    tw = fm.horizontalAdvance(manager->userFlowItems[i]->caption);
    painter.drawText(50 - tw / 2, 10 + th / 2, manager->userFlowItems[i]->caption);
    p_item->setIcon(px);
    p_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
    p_lstIO->addItem(p_item);
    p_lstIO->setItemWidget(p_item, new CustomListItem(FlowItemType::userdefined, px, tr("Test"), 4131 + i));
  }

  QTreeWidgetItem *p_itemIO_Import = new QTreeWidgetItem;
  p_itemIO_Import->setDisabled(true);
  p_catIO->addChild(p_itemIO_Import);
  setItemWidget(p_itemIO_Import, 0, p_lstIO);

  painter.end();

  expandAll();
}

/***** User Items List Widget *****/

UserItemsListWidget::UserItemsListWidget(QWidget *parent) : QListWidget(parent) {}

void UserItemsListWidget::startDrag(Qt::DropActions supportedActions) {
  if (!(supportedActions & Qt::CopyAction)) {
    QListWidget::startDrag(supportedActions);
    return;
  }

  QListWidgetItem *p_wgt = currentItem();
  if (!p_wgt)
    return;

  CustomListItem* p_item = static_cast<CustomListItem*>(itemWidget(p_wgt));
  QPixmap px = p_item->GetPixmap();
  QByteArray itemData;
  QDataStream dataStream(&itemData, QIODevice::WriteOnly);
  dataStream << p_item->GetType() << p_item->GetUniueID();

  QMimeData *mimeData = new QMimeData;
  mimeData->setData("application/x-dndflowitemdata", itemData);

  QDrag* drag = new QDrag(this);
  drag->setPixmap(px);
  drag->setMimeData(mimeData);
  drag->setHotSpot(px.rect().center());
  drag->exec(Qt::CopyAction);
}


#include "navlistview.h"

NavListView::NavListView(QWidget *parent) : QListWidget(parent)
{
    deviceItem = new NavListItem(QIcon(":/images/device.png"), "Device", this);
    NavListItem *musicItem = new NavListItem(QIcon(":/images/musics.png"), "Music", this);
    NavListItem *fileItem = new NavListItem(QIcon(":/images/files.png"), "File", this);

    CurrentDeviceManager *cdm = CurrentDeviceManager::getInstance();
    connect(cdm, &CurrentDeviceManager::currentDeviceChanged, this, &NavListView::setDeviceName);
    connect(cdm, &CurrentDeviceManager::allDevicePlugOuted, this, &NavListView::clearDeviceName);

    this->addItem(deviceItem);
    this->addItem(musicItem);
    this->addItem(fileItem);

    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->setFixedWidth(200);
    this->setContentsMargins(0, 0, 0, 0);

    // 点击导航栏时更改导航样式
    connect(this, &NavListView::currentItemChanged, this, &NavListView::onCurrentItemChanged);
}

void NavListView::onCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (previous) {
        static_cast<NavListItem*>(previous)->setDefaultStyle();
    }
    static_cast<NavListItem*>(current)->setSelectedStyle();
}

void NavListView::setDeviceName(Device *device)
{
    deviceItem->setText(device->getModel());
}

void NavListView::clearDeviceName()
{
    deviceItem->setText("Device");
}

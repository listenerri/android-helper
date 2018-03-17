#include "rightstackwidget.h"
#include "currentdevicemanager.h"

RightStackWidget::RightStackWidget(QWidget *parent) : QStackedWidget(parent)
{
    devicePage = new DevicePage(this);

    this->addWidget(devicePage);

    CurrentDeviceManager *cdm = CurrentDeviceManager::getInstance();
    connect(cdm, &CurrentDeviceManager::currentDeviceChanged, this, &RightStackWidget::onCurrentDeviceChanged);
    connect(cdm, &CurrentDeviceManager::deviceUnMounted, this, &RightStackWidget::onDeviceUnMounted);
}

RightStackWidget::~RightStackWidget()
{

}

void RightStackWidget::onCurrentDeviceChanged()
{
    musicPage = new MusicPage(this);
    filePage = new FilePage(this);
    this->addWidget(musicPage);
    this->addWidget(filePage);
}

void RightStackWidget::onDeviceUnMounted()
{
    this->removeWidget(musicPage);
    this->removeWidget(filePage);
    if (this->count() > 1) {
        delete musicPage;
        delete filePage;
    }
}

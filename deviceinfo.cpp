#include <QVBoxLayout>
#include "deviceinfo.h"

DeviceInfo::DeviceInfo(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    deviceModel = new QLabel("Device Model", this);
    deviceBrand = new QLabel("Device Brand", this);
    deviceAndroidRelease = new QLabel("Device Android Release", this);
    connectButton = new QPushButton("connect", this);

    mainLayout->addWidget(deviceModel);
    mainLayout->addWidget(deviceBrand);
    mainLayout->addWidget(deviceAndroidRelease);
    mainLayout->addWidget(connectButton);

    connect(connectButton, &QPushButton::clicked, this, &DeviceInfo::onButtonClicked);
    CurrentDeviceManager *CDM = CurrentDeviceManager::getInstance();
    connect(CDM, &CurrentDeviceManager::currentDeviceChanged, this, &DeviceInfo::setDeviceInfo);
    connect(CDM, &CurrentDeviceManager::deviceUnMounted, this, &DeviceInfo::clearDeviceInfo);

    this->setLayout(mainLayout);
}

void DeviceInfo::onButtonClicked()
{
    CurrentDeviceManager *CDM = CurrentDeviceManager::getInstance();
    if (CDM->getCurrentDevice()->getIsMounted()) {
        CDM->getMtpTool()->unmount();
    } else {
        CDM->getMtpTool()->mount();
    }
}

void DeviceInfo::setDeviceInfo(Device *device)
{
    deviceModel->setText(device->getModel());
    deviceBrand->setText(device->getBrand());
    deviceAndroidRelease->setText("Android: " + device->getAndroidRelease());
    connectButton->setText("disconnect");
}

void DeviceInfo::clearDeviceInfo()
{
    deviceModel->setText("Device Model");
    deviceBrand->setText("Device Brand");
    deviceAndroidRelease->setText("Device Android Release");
    connectButton->setText("connect");
}

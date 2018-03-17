#include "devicepage.h"
#include <QVBoxLayout>
#include <currentdevicemanager.h>

DevicePage::DevicePage(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainVBoxLayout = new QVBoxLayout(this);
    m_tipLabel = new QLabel("Waiting for device connect...", this);
    m_seqView = new DPictureSequenceView;
    m_seqView->setPictureSequence(":/images/Spinner/Spinner%1.png", QPair<int, int>(1, 91), 2);
    m_seqView->setFixedSize(50, 50);
    m_seqView->setSpeed(20);
    m_seqView->play();

    CurrentDeviceManager *CDM = CurrentDeviceManager::getInstance();
    connect(CDM, &CurrentDeviceManager::currentDeviceChanged, this, &DevicePage::onCurrentDeviceChanged);
    connect(CDM, &CurrentDeviceManager::deviceUnMounted, this, &DevicePage::onDeviceUnMounted);

    mainVBoxLayout->addStretch(1);
    mainVBoxLayout->addWidget(m_tipLabel, 0, Qt::AlignCenter);
    mainVBoxLayout->addWidget(m_seqView, 0, Qt::AlignCenter);
    mainVBoxLayout->addStretch(1);

    this->setLayout(mainVBoxLayout);
}

void DevicePage::onCurrentDeviceChanged(Device *currentDevice)
{
    m_tipLabel->setText("device " + currentDevice->getBrand() + " " + currentDevice->getModel() + " is connected :)");
    m_seqView->stop();
    m_seqView->hide();
}

void DevicePage::onDeviceUnMounted()
{
    m_tipLabel->setText("Waiting for device connect...");
    m_seqView->play();
    m_seqView->show();
}

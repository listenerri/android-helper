#include "filepagefiletreesd.h"
#include "currentdevicemanager.h"
#include <QLabel>

FilePageFileTreeSD::FilePageFileTreeSD(QWidget *parent) : FilePageFileTree(parent)
{
    Device *currentDevice = CurrentDeviceManager::getInstance()->getCurrentDevice();
    if (currentDevice->getHasSDCard()) {
        treeView->setRootIndex(fileSystemModel->index(currentDevice->getSdcardPath()));
    } else {
        QLabel *noSdcard = new QLabel("The device has no sdcard.", this);
        treeView->setHidden(true);
        mainVBLayout->addWidget(noSdcard, 0, Qt::AlignCenter);
    }
}

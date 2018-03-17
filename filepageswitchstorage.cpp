#include "filepageswitchstorage.h"
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QPushButton>

FilePageSwitchStorage::FilePageSwitchStorage(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *mainHBlayout = new QHBoxLayout(this);
    QPushButton *storageButton = new QPushButton("device", this);
    QPushButton *sdcardButton = new QPushButton("sdcard", this);

    mainHBlayout->addWidget(storageButton);
    mainHBlayout->addWidget(sdcardButton);
    mainHBlayout->setSpacing(0);
    this->setLayout(mainHBlayout);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(storageButton, 0);
    buttonGroup->addButton(sdcardButton, 1);
    connect(buttonGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),\
            this, &FilePageSwitchStorage::onButtonClicked);
}

void FilePageSwitchStorage::onButtonClicked(int id)
{
    emit switchStoragePage(id);
}

#include <QVBoxLayout>
#include <QStackedWidget>
#include "filepage.h"
#include "filepageswitchstorage.h"
#include "splitline.h"

FilePage::FilePage(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainVBLayout = new QVBoxLayout(this);

    m_toolsBar = new ToolsBar(this);
    FilePageSwitchStorage *switchStorage = new FilePageSwitchStorage(this);
    m_toolsBar->addCenterWidget(switchStorage);

    QStackedWidget *stackPages = new QStackedWidget(this);
    m_fileTree = new FilePageFileTree;
    m_fileTreeSD = new FilePageFileTreeSD;
    stackPages->addWidget(m_fileTree);
    stackPages->addWidget(m_fileTreeSD);
    stackPages->setCurrentIndex(0);

    connect(switchStorage, &FilePageSwitchStorage::switchStoragePage, stackPages, &QStackedWidget::setCurrentIndex);
    connect(stackPages, &QStackedWidget::currentChanged, this, &FilePage::resetPagesConnect);
    connect(m_toolsBar, &ToolsBar::deleteButtonClicked, m_fileTree, &FilePageFileTree::deleteSelected);

    mainVBLayout->setSpacing(0);
    mainVBLayout->setContentsMargins(0, 0, 0, 0);

    mainVBLayout->addWidget(m_toolsBar);
    mainVBLayout->addWidget(new SplitLine(SplitLine::Horizontal, this));
    mainVBLayout->addWidget(stackPages);

    this->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainVBLayout);
}

// 确保只有当前page与toolsbar中的删除按钮进行信号槽连接
void FilePage::resetPagesConnect(int currentPageIndex)
{
    disconnect(m_toolsBar, &ToolsBar::deleteButtonClicked, 0, 0);

    switch (currentPageIndex) {
    case 0:
        connect(m_toolsBar, &ToolsBar::deleteButtonClicked, m_fileTree, &FilePageFileTree::deleteSelected);
        break;
    case 1:
        connect(m_toolsBar, &ToolsBar::deleteButtonClicked, m_fileTreeSD, &FilePageFileTree::deleteSelected);
        break;
    default:
        break;
    }
}

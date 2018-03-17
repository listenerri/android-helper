#include "centralwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "leftsidebar.h"
#include "splitline.h"
#include "rightstackwidget.h"
#include "navlistview.h"

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainVBLayout = new QVBoxLayout(this);
    QHBoxLayout *contentHBLayout = new QHBoxLayout;
    LeftSideBar *leftSideBar = new LeftSideBar(this);
    RightStackWidget *rightStackWidget = new RightStackWidget(this);

    contentHBLayout->addWidget(leftSideBar);
    contentHBLayout->addWidget(new SplitLine(SplitLine::Vertical, this));
    contentHBLayout->addWidget(rightStackWidget);

    contentHBLayout->setMargin(0);
    contentHBLayout->setContentsMargins(0, 0, 0, 0);
    contentHBLayout->setSpacing(0);

    mainVBLayout->setMargin(0);
    mainVBLayout->setContentsMargins(0, 0, 0, 0);
    mainVBLayout->setSpacing(0);

    mainVBLayout->addWidget(new SplitLine(SplitLine::Horizontal, this));
    mainVBLayout->addLayout(contentHBLayout);

    // 当切换左边的导航栏时切换右边的pages
    connect(leftSideBar->navListView, &NavListView::currentRowChanged, rightStackWidget, &RightStackWidget::setCurrentIndex);

}

CentralWidget::~CentralWidget()
{
}

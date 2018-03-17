#include "toolsbar.h"
#include <QPushButton>

ToolsBar::ToolsBar(QWidget *parent) : QWidget(parent)
{
    mainGridLayout = new QGridLayout(this);

    QPushButton *deleteButton = new QPushButton(this);
    deleteButton->setIcon(QIcon(":/images/delete.png"));

    connect(deleteButton, &QPushButton::clicked, this, &ToolsBar::onDeleteButtonClicked);

    mainGridLayout->setColumnStretch(0, 1);
    mainGridLayout->setColumnStretch(1, 1);
    mainGridLayout->setColumnStretch(2, 1);
    mainGridLayout->setSpacing(0);
    mainGridLayout->setContentsMargins(10, 0, 10, 0);

    mainGridLayout->addWidget(deleteButton, 0, 2, Qt::AlignRight | Qt::AlignVCenter);

    this->setLayout(mainGridLayout);
    this->setContentsMargins(0, 0, 0, 0);
}

void ToolsBar::addLeftWidget(QWidget *leftWidget)
{
    mainGridLayout->addWidget(leftWidget, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
}

void ToolsBar::addCenterWidget(QWidget *centerWidget)
{
    mainGridLayout->addWidget(centerWidget, 0, 1, Qt::AlignCenter);
}

void ToolsBar::onDeleteButtonClicked()
{
    emit deleteButtonClicked();
}

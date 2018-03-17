#include "navlistitem.h"

NavListItem::NavListItem(const QIcon &icon, const QString &text, QListWidget *parent) : QListWidgetItem(icon, text, parent)
{
    this->setSizeHint(QSize(0, 40));
    this->setDefaultStyle();
}

void NavListItem::setDefaultStyle()
{
    QFont defaultFont;
    defaultFont.setBold(false);
    this->setFont(defaultFont);
    this->setBackgroundColor(QColor("#fff"));
}

void NavListItem::setSelectedStyle()
{
    QFont selectedFont;
    selectedFont.setBold(true);
    this->setFont(selectedFont);
    this->setBackgroundColor(QColor("#f5f6f7"));
}

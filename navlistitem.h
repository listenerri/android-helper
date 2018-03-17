#ifndef LEFTLISTITEM_H
#define LEFTLISTITEM_H

#include <QListWidgetItem>

class NavListItem : public QListWidgetItem
{
public:
    NavListItem(const QIcon &icon, const QString &text, QListWidget *parent = nullptr);

public:
    void setDefaultStyle();
    void setSelectedStyle();

};

#endif // LEFTLISTITEM_H

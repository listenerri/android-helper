#ifndef LEFTLISTVIEW_H
#define LEFTLISTVIEW_H

#include "navlistitem.h"
#include <QListWidget>
#include "currentdevicemanager.h"

class NavListView : public QListWidget
{
public:
    explicit NavListView(QWidget *parent = nullptr);

public slots:
    void onCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    NavListItem *deviceItem;

private slots:
    void setDeviceName(Device *device);
    void clearDeviceName();
};

#endif // LEFTLISTVIEW_H

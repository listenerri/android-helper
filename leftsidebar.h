#ifndef LEFTVBOXLAYOUT_H
#define LEFTVBOXLAYOUT_H

#include <QWidget>
#include "navlistview.h"
#include "deviceinfo.h"

class LeftSideBar : public QWidget
{
    Q_OBJECT
public:
    explicit LeftSideBar(QWidget *parent = nullptr);
    ~LeftSideBar();

public:
    NavListView *navListView;
    DeviceInfo *deviceInfo;

private:
};

#endif // LEFTVBOXLAYOUT_H

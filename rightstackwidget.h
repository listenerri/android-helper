#ifndef RIGHTSTACKWIDGET_H
#define RIGHTSTACKWIDGET_H

#include <QStackedWidget>
#include "devicepage.h"
#include "musicpage.h"
#include "filepage.h"

class RightStackWidget : public QStackedWidget
{
public:
    RightStackWidget(QWidget *parent = nullptr);
    ~RightStackWidget();

private:
    DevicePage *devicePage;
    MusicPage *musicPage;
    FilePage *filePage;

private slots:
    void onCurrentDeviceChanged();
    void onDeviceUnMounted();
};

#endif // RIGHTSTACKWIDGET_H

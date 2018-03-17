#ifndef NODEVICEPAGE_H
#define NODEVICEPAGE_H

#include <QWidget>
#include <QLabel>
#include <dpicturesequenceview.h>
#include "tools/device.h"

DWIDGET_USE_NAMESPACE

class DevicePage : public QWidget
{
    Q_OBJECT
public:
    explicit DevicePage(QWidget *parent = nullptr);

signals:

private slots:
    void onCurrentDeviceChanged(Device *currentDevice);
    void onDeviceUnMounted();

private:
    QLabel *m_tipLabel;
    DPictureSequenceView *m_seqView;
};

#endif // NODEVICEPAGE_H

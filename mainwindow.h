#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include <ddialog.h>
#include "currentdevicemanager.h"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(DMainWindow *parent = 0);
    ~MainWindow();

private:
    DDialog *m_dialogNewDeviceStateInvalid;
    DDialog *m_dialogDetectDeviceFailed;
    DDialog *m_dialogDeviceMountFailed;
    DDialog *m_dialogMountProcessExecFailed;
    DDialog *m_dialogUnmountProcessExecFailed;
    DDialog *m_dialogFindFiles;
    DDialog *m_dialogFilesDeleteFailed;

private:
    void initDialogs();

private slots:
    void onNewDeviceStateInvalid();
    void onDetectDeviceFailed();
    void onDeviceMountFailed(QString why);
    void onMountProcessExecFailed();
    void onUnMountProcessExecFailed();
    void showFindFilesDialog();
    void dismissFindFilesDialog();
    void onMusicFilesDeletedFailed(QStringList failedFilesPath);
};

#endif // MAINWINDOW_H

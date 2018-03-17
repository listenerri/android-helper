#include <QPushButton>
#include "mainwindow.h"
#include "centralwidget.h"
#include <dpicturesequenceview.h>

DWIDGET_USE_NAMESPACE

MainWindow::MainWindow(DMainWindow *parent) :
    DMainWindow(parent)
{
    CentralWidget *centralWidget = new CentralWidget(this);
    setCentralWidget(centralWidget);

    initDialogs();

    CurrentDeviceManager *CDM = CurrentDeviceManager::getInstance();
    // 错误提示信号槽
    connect(CDM, &CurrentDeviceManager::newDeviceStateInvalid, this, &MainWindow::onNewDeviceStateInvalid);
    connect(CDM, &CurrentDeviceManager::detectDeviceFailed, this, &MainWindow::onDetectDeviceFailed);
    connect(CDM, &CurrentDeviceManager::deviceMountFailed, this, &MainWindow::onDeviceMountFailed);
    connect(CDM, &CurrentDeviceManager::mountProcessExecFailed, this, &MainWindow::onMountProcessExecFailed);
    connect(CDM, &CurrentDeviceManager::unmountProcessExecFailed, this, &MainWindow::onUnMountProcessExecFailed);
    connect(CDM->getCurrentDevice(), &Device::musicFilesDeletedFailed, this, &MainWindow::onMusicFilesDeletedFailed);

    // 加载文件信号槽
    connect(CDM, &CurrentDeviceManager::startFindFiles, this, &MainWindow::showFindFilesDialog);
    connect(CDM, &CurrentDeviceManager::currentDeviceChanged, this, &MainWindow::dismissFindFilesDialog);
    connect(m_dialogFindFiles, &DDialog::buttonClicked, CDM, &CurrentDeviceManager::cancelFindFiles);
}

MainWindow::~MainWindow()
{
}

void MainWindow::initDialogs()
{
    m_dialogNewDeviceStateInvalid = new DDialog(this);
    m_dialogNewDeviceStateInvalid->addButton("OK", false, DDialog::ButtonNormal);

    m_dialogDetectDeviceFailed = new DDialog(this);
    m_dialogDetectDeviceFailed->addButton(QString("I'see"), false, DDialog::ButtonWarning);

    m_dialogDeviceMountFailed = new DDialog(this);
    m_dialogDeviceMountFailed->addButton(QString("OK"), false, DDialog::ButtonWarning);

    m_dialogMountProcessExecFailed = new DDialog(this);
    m_dialogMountProcessExecFailed->addButton(QString("OK"), false, DDialog::ButtonWarning);

    m_dialogUnmountProcessExecFailed = new DDialog(this);
    m_dialogUnmountProcessExecFailed->addButton(QString("OK"), false, DDialog::ButtonWarning);

    m_dialogFindFiles = new DDialog(this);
    DPictureSequenceView *seqView = new DPictureSequenceView;
    seqView->setPictureSequence(":/images/Spinner/Spinner%1.png", QPair<int, int>(1, 91), 2);
    seqView->setFixedSize(50, 50);
    seqView->setSpeed(20);
    m_dialogFindFiles->addContent(seqView, Qt::AlignCenter);
    m_dialogFindFiles->addButton(QString("Cancel"), false, DDialog::ButtonWarning);

    m_dialogFilesDeleteFailed = new DDialog(this);
    m_dialogFilesDeleteFailed->addButton(QString("OK"), false, DDialog::ButtonWarning);
}

// 设备状态警告
void MainWindow::onNewDeviceStateInvalid()
{
    m_dialogNewDeviceStateInvalid->setTitle(QString("Note:"));
    m_dialogNewDeviceStateInvalid->setMessage(QString("The newly inserted device status is not correct !\n\n"
                                                     "which may be because the device did not grant the computer\n"
                                                     "connection authority or the adb program did not run correctly."));
    m_dialogNewDeviceStateInvalid->open();
}

// adb程序出错
void MainWindow::onDetectDeviceFailed()
{
    m_dialogDetectDeviceFailed->setTitle(QString("Warning:"));
    m_dialogDetectDeviceFailed->setMessage(QString("Unable to detect the device !\n\n"
                                                  "Which may be because adb programs are not installed\n"
                                                  "or are not working properly.\n\n"
                                                  "You need to check out adb program and restart this program."));
    m_dialogDetectDeviceFailed->exec();
}

// 挂载设备异常
void MainWindow::onDeviceMountFailed(QString why)
{
    m_dialogDeviceMountFailed->setTitle(QString("Warning:"));
    m_dialogDeviceMountFailed->setMessage(QString("Unable to mount the device: " + why + "\n\n"
                                                  "Which may be because your device is NOT UNLOCK\n"
                                                  "or your device current connect mode IS NOT MTP.\n\n"
                                                  "After check the above two messages you can click CONNECT button\n"
                                                  "to remount your device."));
    m_dialogDeviceMountFailed->open();
}

// 挂载程序异常
void MainWindow::onMountProcessExecFailed()
{
    m_dialogMountProcessExecFailed->setTitle(QString("Warning:"));
    m_dialogMountProcessExecFailed->setMessage(QString("Unable to mount the device !\n\n"
                                                  "Which may be because go-mtpfs programs are not installed\n"
                                                  "or are not working properly.\n\n"
                                                  "You need to check out go-mtpfs program."));
    m_dialogMountProcessExecFailed->exec();
}

// 卸载程序异常
void MainWindow::onUnMountProcessExecFailed()
{
    m_dialogUnmountProcessExecFailed->setTitle(QString("Warning:"));
    m_dialogUnmountProcessExecFailed->setMessage(QString("Unable to unmount the device !\n\n"
                                                  "Which may be because fusermount programs are not installed\n"
                                                  "or are not working properly.\n\n"
                                                  "You need to check out fusermount program."));
    m_dialogUnmountProcessExecFailed->exec();
}

// 正在加载设备文件
void MainWindow::showFindFilesDialog()
{
    m_dialogFindFiles->setTitle(QString("Loading device files..."));
    DPictureSequenceView *seqView = static_cast<DPictureSequenceView *>(m_dialogFindFiles->getContent(0));
    seqView->play();
    m_dialogFindFiles->open();
}

void MainWindow::dismissFindFilesDialog()
{
    m_dialogFindFiles->accept();
    DPictureSequenceView *seqView = static_cast<DPictureSequenceView *>(m_dialogFindFiles->getContent(0));
    seqView->stop();
}

void MainWindow::onMusicFilesDeletedFailed(QStringList failedFilesPath)
{
    m_dialogFilesDeleteFailed->setTitle(QString("These files delete failed:"));
    QString message;
    foreach (QString path, failedFilesPath) {
        message += QFileInfo(path).fileName() + "\n";
    }
    m_dialogFilesDeleteFailed->setMessage(message);
    m_dialogFilesDeleteFailed->open();
}

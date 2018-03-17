#include "deviceprovider.h"
#include <QDebug>

DeviceProvider::DeviceProvider(QObject *parent) : QObject(parent)
{
    m_timerId = startTimer(3000);
    connect(&m_adbTool, &AdbTool::deviceConnectInfoReady, this, &DeviceProvider::onDeviceConnectInfoReady);
    connect(&m_adbTool, &AdbTool::processExecFailed, this, &DeviceProvider::onAdbProcessExecFailed);
    connect(&m_adbTool, &AdbTool::deviceDetailInfoReady, this, &DeviceProvider::onDeviceDetailInfoReady);
}

void DeviceProvider::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    // 检测设备插入和拔出
    m_adbTool.startProcess(AdbTool::ProcessType::DetectDevice);
}

// 处理设备插入和拔出
void DeviceProvider::onDeviceConnectInfoReady(QStringList deviceIndicatorList)
{
    int m_deviceCount = m_devices.count();
    int deviceCount = deviceIndicatorList.count();

    // 有可能以比较快的速度插入或拔出了多个设备, 遍历一下为好
    // 有设备插入
    if (m_deviceCount < deviceCount) {
        // 如果当前没有已插入的设备则直接添加新设备
        if (m_deviceCount == 0) {
            for (int i = 0; i < deviceCount; ++i) {
                addDevice(deviceIndicatorList[i]);
            }
        }
        // 如果当前有已插入的设备
        else {
            // 先过滤掉已插入的设备
            for (int i = 0; i < m_deviceCount; ++i) {
                if (deviceIndicatorList.contains(m_devices[i].getDeviceIndicator())) {
                    deviceIndicatorList.removeAll(m_devices[i].getDeviceIndicator());
                }
            }
            // 再添加新设备
            for (int i = 0; i < deviceIndicatorList.count(); ++i) {
                addDevice(deviceIndicatorList[i]);
            }
        }
    }
    // 有设备拔出
    else if (m_deviceCount > deviceCount) {
        // TODO: 使用遍历器完成移除设备的操作
        // 找到已拔出的设备对象
        QList<Device> plugoutedDevices;
        for (int i = 0; i < m_deviceCount; ++i) {
            if (!deviceIndicatorList.contains(m_devices[i].getDeviceIndicator())) {
                plugoutedDevices << m_devices[i];
            }
        }
        // 移除设备
        if (!plugoutedDevices.isEmpty()) {
            foreach (auto device, plugoutedDevices) {
                qDebug() << "(DeviceProvider::onDeviceConnectInfoReady) device:" << device.getDeviceIndicator() << "plug out";
                m_devices.removeAll(device);
            }
        }
        // 同时或快速移除多个设备时只激活一次信号
        if (m_deviceCount != m_devices.count()) {
            qDebug() << "(DeviceProvider::onDeviceConnectInfoReady) emit deviceCountChanged";
            emit deviceCountChanged(m_devices);
        }
    }
}

void DeviceProvider::onAdbProcessExecFailed(AdbTool::ProcessType processType)
{
    // 如果adb命令执行失败激活相关信号并停止timer
    qDebug() << "(DeviceProvider::onAdbProcessExecFailed) emit adbProcessExecFailed!!!";
    emit detectDeviceFailed();
    killTimer(m_timerId);

    switch (processType) {
    case AdbTool::DetectDevice:
        break;
    case AdbTool::GetDeviceDetailInfo:
        // 如果adb命令是在获取设备详细信息时执行失败则移除这个已添加但信息不完整的设备
        if (!m_devices.isEmpty()) {
            m_devices.removeLast();
        }
        break;
    default:
        break;
    }
}

void DeviceProvider::onDeviceDetailInfoReady(QMap<QString, QString> detailInfoMap)
{
    m_devices.last().setModel(detailInfoMap.value("ro.product.model"));
    m_devices.last().setBrand(detailInfoMap.value("ro.product.brand"));
    m_devices.last().setAndroidRelease(detailInfoMap.value("ro.build.version.release"));
    m_devices.last().setDetailInfo(detailInfoMap);
    qDebug() << "(DeviceProvider::onDeviceDetailInfoReady) device:" << m_devices.last().getDeviceIndicator() << "plug in";
    qDebug() << "(DeviceProvider::onDeviceDetailInfoReady) emit deviceCountChanged";
    emit deviceCountChanged(m_devices);
}

void DeviceProvider::addDevice(QString deviceIndicator)
{
    QString serialNumber = getSerialNumber(deviceIndicator);
    // 如果设备状态正确添加这个设备
    if (isDeviceStateValid(deviceIndicator)) {
        // 如果一个设备的状态变得正确了就从状态不正常的SerialNumber列表里将其SerialNumber移除
        if (m_stateInvalidSNList.contains(serialNumber)) {
            qDebug() << "(DeviceProvider::addDevice) device:" << serialNumber<< "state is valid now";
            m_stateInvalidSNList.removeAll(serialNumber);
        }
        // 增加一个信息不完整的设备(目前只有deviceIndicator属性),并开始获取其他设备详细信息,
        // 等待设备详细信息在onDeviceDetailInfoReady槽中获取到之后再在这个槽中激活增加了一个设备的信号
        Device newDevice(deviceIndicator);
        m_devices.append(newDevice);
        m_adbTool.startProcess(AdbTool::GetDeviceDetailInfo, serialNumber);
    } else {
        // 如果设备状态不正常, 且没有保存其SerialNumber则将其保存起来,
        // 以免每次检测都会激活同一个设备状态不正确的信号
        if (!m_stateInvalidSNList.contains(serialNumber)) {
            qDebug() << "(DeviceProvider::addDevice) device:" << serialNumber<< "state is invalid";
            qDebug() << "(DeviceProvider::addDevice) emit newDeviceStateInvalid";
            m_stateInvalidSNList << serialNumber;
            emit newDeviceStateInvalid();
        } else {
            qDebug() << "(DeviceProvider::addDevice) device:" << serialNumber<< "state is still invalid";
        }
    }
}

QStringList DeviceProvider::splitIndicator(QString deviceIndicator)
{
    // 返回serial number和device state组成的列表
    QStringList deviceIndicatorSplited = deviceIndicator.split("\t", QString::SkipEmptyParts);
    return deviceIndicatorSplited;
}

QString DeviceProvider::getSerialNumber(QString deviceIndicator)
{
    return splitIndicator(deviceIndicator)[0];
}

bool DeviceProvider::isDeviceStateValid(QString deviceIndicator)
{
    // 当不是以"device"结尾时, 可能是手机没有授权电脑usb调试权限, 或者adb server权限不正确
    return splitIndicator(deviceIndicator)[1] == "device";
}

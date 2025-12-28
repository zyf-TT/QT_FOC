#include "SerialPortManager.h"
#include <QDebug>

SerialPortManager::SerialPortManager(QObject *parent)
    : QObject(parent)
    , serialPort(new QSerialPort(this))
    , refreshTimer(new QTimer(this))
    , currentPortName("")
    , currentBaudRate(115200)
    , readBuffer()
{
    setupSerialPort();
}

SerialPortManager::~SerialPortManager()
{
    if (serialPort->isOpen()) {
        serialPort->close();
    }
}

void SerialPortManager::setupSerialPort()
{
    // 连接信号
    connect(serialPort, &QSerialPort::readyRead, this, &SerialPortManager::handleReadyRead);
    connect(serialPort, &QSerialPort::errorOccurred, this, &SerialPortManager::handleError);
}

bool SerialPortManager::openPort(const QString &portName, int baudRate)
{
    if (serialPort->isOpen()) {
        closePort();
    }
    
    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    
    if (serialPort->open(QIODevice::ReadWrite)) {
        currentPortName = portName;
        currentBaudRate = baudRate;
        emit portOpened(portName, baudRate);
        return true;
    } else {
        emit errorOccurred(serialPort->errorString());
        return false;
    }
}

void SerialPortManager::closePort()
{
    if (serialPort->isOpen()) {
        serialPort->close();
        readBuffer.clear();  // 关闭时清空缓冲区
        emit portClosed();
    }
}

bool SerialPortManager::isPortOpen() const
{
    return serialPort->isOpen();
}

bool SerialPortManager::sendData(const QByteArray &data)
{
    if (!serialPort->isOpen()) {
        emit errorOccurred("串口未打开");
        return false;
    }
    
    qint64 bytesWritten = serialPort->write(data);
    if (bytesWritten == -1) {
        emit errorOccurred("发送数据失败: " + serialPort->errorString());
        return false;
    }
    return true;
}

bool SerialPortManager::sendCommand(const QString &command)
{
    return sendData(command.toUtf8());
}

QStringList SerialPortManager::getAvailablePorts() const
{
    QStringList ports;
    QList<QSerialPortInfo> portInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : portInfos) {
        ports << port.portName();
    }
    return ports;
}

QStringList SerialPortManager::getBaudRateList() const
{
    return {"9600", "19200", "38400", "57600", "115200", "230400", "460800", "921600"};
}

QString SerialPortManager::getCurrentPortName() const
{
    return currentPortName;
}

int SerialPortManager::getCurrentBaudRate() const
{
    return currentBaudRate;
}

QString SerialPortManager::getConnectionString() const
{
    if (isPortOpen()) {
        return QString("Connected: %1 @ %2 bps").arg(currentPortName).arg(currentBaudRate);
    }
    return "Disconnected";
}

void SerialPortManager::handleReadyRead()
{
    if (serialPort->isOpen()) {
        QByteArray newData = serialPort->readAll();
        readBuffer.append(newData);
        
        // 按换行符分割消息（支持 \n 和 \r\n）
        while (readBuffer.contains('\n')) {
            int lineEnd = readBuffer.indexOf('\n');
            QByteArray line = readBuffer.left(lineEnd);
            
            // 移除可能的 \r
            if (line.endsWith('\r')) {
                line.chop(1);
            }
            
            // 移除缓冲区中已处理的数据（包括换行符）
            readBuffer.remove(0, lineEnd + 1);
            
            // 如果行不为空，发送数据
            if (!line.isEmpty()) {
                emit dataReceived(line);
            }
        }
        
        // 如果缓冲区过大（超过10KB），清空缓冲区防止内存溢出
        if (readBuffer.size() > 10240) {
            qDebug() << "接收缓冲区过大，清空缓冲区";
            readBuffer.clear();
        }
    }
}

void SerialPortManager::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        emit errorOccurred(serialPort->errorString());
        if (serialPort->isOpen()) {
            closePort();
        }
    }
}


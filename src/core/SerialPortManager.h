#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QStringList>

class SerialPortManager : public QObject
{
    Q_OBJECT

public:
    explicit SerialPortManager(QObject *parent = nullptr);
    ~SerialPortManager();

    // 串口操作
    bool openPort(const QString &portName, int baudRate);
    void closePort();
    bool isPortOpen() const;
    
    // 数据发送
    bool sendData(const QByteArray &data);
    bool sendCommand(const QString &command);
    
    // 端口列表
    QStringList getAvailablePorts() const;
    QStringList getBaudRateList() const;
    
    // 当前连接信息
    QString getCurrentPortName() const;
    int getCurrentBaudRate() const;
    QString getConnectionString() const;

signals:
    void dataReceived(const QByteArray &data);
    void portOpened(const QString &portName, int baudRate);
    void portClosed();
    void errorOccurred(const QString &errorString);

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort *serialPort;
    QTimer *refreshTimer;
    QString currentPortName;
    int currentBaudRate;
    QByteArray readBuffer;  // 接收缓冲区，用于处理分片数据
    
    void setupSerialPort();
};

#endif // SERIALPORTMANAGER_H


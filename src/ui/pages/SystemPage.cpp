#include "SystemPage.h"
#include "../widgets/Card.h"
#include <QTextCursor>
#include <QTextDocument>
#include <QDateTime>
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QStringConverter>
#else
#include <QTextCodec>
#endif

SystemPage::SystemPage(QWidget *parent)
    : QWidget(parent)
    , isPortOpen(false)
    , connectionState(ConnectionState::Disconnected)
    , logFile(nullptr)
    , logStream(nullptr)
{
    setupUI();
    initializeLogFile();
}

SystemPage::~SystemPage()
{
    closeLogFile();
}

void SystemPage::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);
    
    // 串口设置卡片
    Card *serialCard = new Card("Serial Port Settings", this);
    QVBoxLayout *serialLayout = serialCard->getContentLayout();
    
    QHBoxLayout *portLayout = new QHBoxLayout();
    portLayout->addWidget(new QLabel("端口:", this));
    comboBox_Port = new QComboBox(this);
    comboBox_Port->setMinimumWidth(200);
    portLayout->addWidget(comboBox_Port);
    portLayout->addStretch();
    serialLayout->addLayout(portLayout);
    
    QHBoxLayout *baudLayout = new QHBoxLayout();
    baudLayout->addWidget(new QLabel("波特率:", this));
    comboBox_BaudRate = new QComboBox(this);
    comboBox_BaudRate->setMinimumWidth(150);
    baudLayout->addWidget(comboBox_BaudRate);
    baudLayout->addStretch();
    serialLayout->addLayout(baudLayout);
    
    // 状态标签
    QHBoxLayout *statusLayout = new QHBoxLayout();
    statusLayout->addWidget(new QLabel("状态:", this));
    statusLabel = new QLabel("未连接", this);
    statusLabel->setStyleSheet("font-weight: 500; color: #8e8e93;");
    statusLayout->addWidget(statusLabel);
    statusLayout->addStretch();
    serialLayout->addLayout(statusLayout);
    
    pushButton_OpenClose = new QPushButton("打开串口", this);
    pushButton_OpenClose->setMinimumHeight(35);
    pushButton_OpenClose->setStyleSheet(
        "QPushButton {"
        "    background-color: #007aff;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 6px;"
        "    font-size: 14px;"
        "    font-weight: 500;"
        "    padding: 8px 20px;"
        "}"
        "QPushButton:hover:enabled {"
        "    background-color: #0051d5;"
        "}"
        "QPushButton:pressed:enabled {"
        "    background-color: #0040a8;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #c7c7cc;"
        "    color: #8e8e93;"
        "}"
    );
    connect(pushButton_OpenClose, &QPushButton::clicked, this, &SystemPage::onOpenCloseClicked);
    serialLayout->addWidget(pushButton_OpenClose);
    
    mainLayout->addWidget(serialCard, 0);  // stretch factor为0，不扩展
    
    // 数据日志卡片
    Card *logCard = new Card("Data Log", this);
    QVBoxLayout *logLayout = logCard->getContentLayout();
    
    textEdit_DataLog = new QTextEdit(this);
    textEdit_DataLog->setReadOnly(true);
    logLayout->addWidget(textEdit_DataLog);
    
    mainLayout->addWidget(logCard, 1);
}

void SystemPage::setPortList(const QStringList &ports)
{
    QSettings settings;
    QString savedPort = settings.value("serial/port", "").toString();
    QString currentPort = comboBox_Port->currentData().toString();
    
    comboBox_Port->clear();
    
    for (const QString &port : ports) {
        comboBox_Port->addItem(port, port);
    }
    
    // 优先恢复当前选择的端口，其次使用保存的端口
    int index = comboBox_Port->findData(currentPort);
    if (index < 0 && !savedPort.isEmpty()) {
        index = comboBox_Port->findData(savedPort);
    }
    if (index >= 0) {
        comboBox_Port->setCurrentIndex(index);
    }
}

void SystemPage::setBaudRateList(const QStringList &baudRates)
{
    QSettings settings;
    QString savedBaudRate = settings.value("serial/baudRate", "115200").toString();
    
    comboBox_BaudRate->clear();
    comboBox_BaudRate->addItems(baudRates);
    
    // 恢复保存的波特率，如果不存在则使用默认值
    int index = comboBox_BaudRate->findText(savedBaudRate);
    if (index >= 0) {
        comboBox_BaudRate->setCurrentIndex(index);
    } else {
        comboBox_BaudRate->setCurrentText("115200");
    }
}

void SystemPage::setPortEnabled(bool enabled)
{
    isPortOpen = enabled;
    connectionState = enabled ? ConnectionState::Connected : ConnectionState::Disconnected;
    updateUI();
}

void SystemPage::setConnectionState(ConnectionState state)
{
    connectionState = state;
    updateUI();
}

void SystemPage::updateUI()
{
    // 更新按钮文本和状态
    switch (connectionState) {
        case ConnectionState::Disconnected:
            pushButton_OpenClose->setText("打开串口");
            pushButton_OpenClose->setEnabled(true);
            pushButton_OpenClose->setStyleSheet(
                "QPushButton {"
                "    background-color: #007aff;"
                "    color: white;"
                "    border: none;"
                "    border-radius: 6px;"
                "    font-size: 14px;"
                "    font-weight: 500;"
                "    padding: 8px 20px;"
                "}"
                "QPushButton:hover {"
                "    background-color: #0051d5;"
                "}"
                "QPushButton:pressed {"
                "    background-color: #0040a8;"
                "}"
            );
            comboBox_Port->setEnabled(true);
            comboBox_BaudRate->setEnabled(true);
            break;
        case ConnectionState::Connecting:
            pushButton_OpenClose->setText("连接中...");
            pushButton_OpenClose->setEnabled(false);
            pushButton_OpenClose->setStyleSheet(
                "QPushButton {"
                "    background-color: #c7c7cc;"
                "    color: #8e8e93;"
                "    border: none;"
                "    border-radius: 6px;"
                "    font-size: 14px;"
                "    font-weight: 500;"
                "    padding: 8px 20px;"
                "}"
            );
            comboBox_Port->setEnabled(false);
            comboBox_BaudRate->setEnabled(false);
            break;
        case ConnectionState::Connected:
            pushButton_OpenClose->setText("关闭串口");
            pushButton_OpenClose->setEnabled(true);
            pushButton_OpenClose->setStyleSheet(
                "QPushButton {"
                "    background-color: #ff3b30;"
                "    color: white;"
                "    border: none;"
                "    border-radius: 6px;"
                "    font-size: 14px;"
                "    font-weight: 500;"
                "    padding: 8px 20px;"
                "}"
                "QPushButton:hover {"
                "    background-color: #d70015;"
                "}"
                "QPushButton:pressed {"
                "    background-color: #b50012;"
                "}"
            );
            comboBox_Port->setEnabled(false);
            comboBox_BaudRate->setEnabled(false);
            break;
        case ConnectionState::Error:
            pushButton_OpenClose->setText("重新连接");
            pushButton_OpenClose->setEnabled(true);
            pushButton_OpenClose->setStyleSheet(
                "QPushButton {"
                "    background-color: #ff9500;"
                "    color: white;"
                "    border: none;"
                "    border-radius: 6px;"
                "    font-size: 14px;"
                "    font-weight: 500;"
                "    padding: 8px 20px;"
                "}"
                "QPushButton:hover {"
                "    background-color: #e6850e;"
                "}"
                "QPushButton:pressed {"
                "    background-color: #cc750c;"
                "}"
            );
            comboBox_Port->setEnabled(true);
            comboBox_BaudRate->setEnabled(true);
            break;
    }
    
    // 更新状态标签
    statusLabel->setText(getStateText(connectionState));
    statusLabel->setStyleSheet(QString("font-weight: 500; color: %1;").arg(getStateColor(connectionState)));
}

QString SystemPage::getStateText(ConnectionState state) const
{
    switch (state) {
        case ConnectionState::Disconnected:
            return "未连接";
        case ConnectionState::Connecting:
            return "连接中...";
        case ConnectionState::Connected:
            return "已连接";
        case ConnectionState::Error:
            return "连接错误";
    }
    return "未知";
}

QString SystemPage::getStateColor(ConnectionState state) const
{
    switch (state) {
        case ConnectionState::Disconnected:
            return "#8e8e93";
        case ConnectionState::Connecting:
            return "#ff9500";
        case ConnectionState::Connected:
            return "#34c759";
        case ConnectionState::Error:
            return "#ff3b30";
    }
    return "#8e8e93";
}

void SystemPage::onOpenCloseClicked()
{
    // 根据连接状态决定操作
    if (connectionState == ConnectionState::Connected) {
        // 当前已连接，执行关闭操作
        emit closePortRequested();
    } else {
        // 当前未连接或处于其他状态，执行打开操作
        QString portName = comboBox_Port->currentData().toString();
        int baudRate = comboBox_BaudRate->currentText().toInt();
        
        if (portName.isEmpty()) {
            // 如果没有选择端口，不执行操作（UI应该已经禁用按钮）
            return;
        }
        
        // 保存串口参数
        QSettings settings;
        settings.setValue("serial/port", portName);
        settings.setValue("serial/baudRate", QString::number(baudRate));
        
        emit openPortRequested(portName, baudRate);
    }
}

QString SystemPage::getSelectedPort() const
{
    return comboBox_Port->currentData().toString();
}

int SystemPage::getSelectedBaudRate() const
{
    return comboBox_BaudRate->currentText().toInt();
}

void SystemPage::appendLog(const QString &message, bool isReceived)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QString prefix = isReceived ? "[接收]" : "[发送]";
    QString color = isReceived ? "#007aff" : "#34c759";
    
    QString formattedMessage = QString("<span style='color: %1;'>[%2] %3 %4</span>")
                              .arg(color, timestamp, prefix, message);
    
    textEdit_DataLog->append(formattedMessage);
    
    // 写入文件（纯文本格式）
    if (logFile && logFile->isOpen() && logStream) {
        QString plainTextLog = QString("[%1] %2 %3\n")
                              .arg(timestamp, prefix, message);
        *logStream << plainTextLog;
        logStream->flush();
    }
    
    // 限制日志行数（最多1000行）
    QTextDocument *doc = textEdit_DataLog->document();
    int blockCount = doc->blockCount();
    if (blockCount > 1000) {
        QTextCursor cursor(doc);
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, blockCount - 1000);
        cursor.movePosition(QTextCursor::StartOfBlock);
        cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
    }
    
    // 自动滚动到底部
    QTextCursor cursor = textEdit_DataLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    textEdit_DataLog->setTextCursor(cursor);
}

void SystemPage::initializeLogFile()
{
    // 获取应用程序目录
    QString appDir = QCoreApplication::applicationDirPath();
    QString logFilePath = QDir(appDir).filePath("foc_data_log.txt");
    
    // 创建并打开文件（WriteOnly模式会清空文件）
    logFile = new QFile(logFilePath, this);
    if (logFile->open(QIODevice::WriteOnly | QIODevice::Text)) {
        logStream = new QTextStream(logFile);
        
        // 设置UTF-8编码（兼容Qt5和Qt6）
        #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        logStream->setEncoding(QStringConverter::Utf8);
        #else
        logStream->setCodec(QTextCodec::codecForName("UTF-8"));
        #endif
        
        // 写入文件头
        QString header = QString("FOC Data Log\n")
                        .append("Started at: ")
                        .append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                        .append("\n")
                        .append("========================================\n");
        *logStream << header;
        logStream->flush();
    } else {
        qDebug() << "无法打开日志文件:" << logFilePath << logFile->errorString();
        delete logFile;
        logFile = nullptr;
    }
}

void SystemPage::closeLogFile()
{
    if (logStream) {
        delete logStream;
        logStream = nullptr;
    }
    
    if (logFile) {
        if (logFile->isOpen()) {
            logFile->close();
        }
        delete logFile;
        logFile = nullptr;
    }
}


#include "MainWindow.h"
#include <QMessageBox>
#include <QApplication>
#include <QScreen>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , serialPortManager(new SerialPortManager(this))
    , dataParser(new DataParser())
    , refreshTimer(new QTimer(this))
{
    // 初始化FOC参数
    focParams = FOCParameters();
    
    // 创建UI
    setupUI();
    
    // 恢复窗口状态
    restoreWindowState();
    
    // 连接信号
    connectSignals();
    
    // 设置定时器用于刷新串口列表
    refreshTimer->setInterval(2000);
    connect(refreshTimer, &QTimer::timeout, this, &MainWindow::refreshPortList);
    refreshTimer->start();
    
    // 初始刷新一次
    refreshPortList();
    
    // 初始化状态
    updateConnectionStatus(false);
}

MainWindow::~MainWindow()
{
    if (serialPortManager->isPortOpen()) {
        serialPortManager->closePort();
    }
    saveWindowState();
}

void MainWindow::setupUI()
{
    // 设置窗口属性
    setWindowTitle("FOC Tuning - Project XYZ");
    
    // 设置窗口大小限制
    setMinimumSize(800, 600);
    setMaximumSize(1920, 1080);
    resize(800, 600);
    
    // 设置样式表
    setStyleSheet(StyleSheet::getApplicationStyleSheet());
    
    // 创建中央部件
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // 主布局
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // 顶部栏
    topBar = new TopBar(this);
    mainLayout->addWidget(topBar);
    
    // 导航栏
    navBar = new NavBar(this);
    mainLayout->addWidget(navBar);
    
    // 页面堆叠
    stackedWidget = new QStackedWidget(this);
    
    // 创建各个页面
    dashboardPage = new DashboardPage(this);
    motorAPage = new MotorAPage(this);
    motorBPage = new MotorBPage(this);
    waveformPage = new WaveformPage(this);
    tuningPage = new TuningPage(this);
    systemPage = new SystemPage(this);
    
    // 添加到堆叠
    stackedWidget->addWidget(dashboardPage);
    stackedWidget->addWidget(motorAPage);
    stackedWidget->addWidget(motorBPage);
    stackedWidget->addWidget(waveformPage);
    stackedWidget->addWidget(tuningPage);
    stackedWidget->addWidget(systemPage);
    
    mainLayout->addWidget(stackedWidget, 1);
    
    // 设置系统页面的波特率列表
    systemPage->setBaudRateList(serialPortManager->getBaudRateList());
    
    // 恢复串口参数（在设置列表之后）
    QSettings settings;
    QString savedPort = settings.value("serial/port", "").toString();
    if (!savedPort.isEmpty()) {
        // 等待端口列表刷新后再尝试恢复
    }
}

void MainWindow::connectSignals()
{
    // 导航栏信号
    connect(navBar, &NavBar::tabClicked, this, &MainWindow::onTabClicked);
    
    // 串口管理器信号
    connect(serialPortManager, &SerialPortManager::dataReceived, this, &MainWindow::onDataReceived);
    connect(serialPortManager, &SerialPortManager::portOpened, this, &MainWindow::onPortOpened);
    connect(serialPortManager, &SerialPortManager::portClosed, this, &MainWindow::onPortClosed);
    connect(serialPortManager, &SerialPortManager::errorOccurred, this, &MainWindow::onPortError);
    
    // Tuning页面信号
    connect(tuningPage, &TuningPage::sendCommandRequested, this, &MainWindow::onSendCommandRequested);
    connect(tuningPage, &TuningPage::startRequested, this, &MainWindow::onStartRequested);
    connect(tuningPage, &TuningPage::stopRequested, this, &MainWindow::onStopRequested);
    
    // System页面信号
    connect(systemPage, &SystemPage::openPortRequested, this, &MainWindow::onOpenPortRequested);
    connect(systemPage, &SystemPage::closePortRequested, this, &MainWindow::onClosePortRequested);
}

void MainWindow::onTabClicked(int index)
{
    if (index >= 0 && index < stackedWidget->count()) {
        stackedWidget->setCurrentIndex(index);
        navBar->setActiveTab(index);
    }
}

void MainWindow::onDataReceived(const QByteArray &data)
{
    // 记录日志
    systemPage->appendLog(QString::fromUtf8(data), true);
    
    // 解析数据
    if (dataParser->parseData(data, focParams)) {
        updateFOCParameters();
    }
}

void MainWindow::onPortOpened(const QString &portName, int baudRate)
{
    topBar->setConnected(portName, baudRate);
    updateConnectionStatus(true);
    systemPage->setPortEnabled(true);
    systemPage->setConnectionState(ConnectionState::Connected);
    systemPage->appendLog(QString("串口已打开: %1 @ %2 baud").arg(portName).arg(baudRate), false);
}

void MainWindow::onPortClosed()
{
    topBar->setDisconnected();
    updateConnectionStatus(false);
    systemPage->setPortEnabled(false);
    systemPage->setConnectionState(ConnectionState::Disconnected);
    systemPage->appendLog("串口已关闭", false);
}

void MainWindow::onPortError(const QString &errorString)
{
    QString friendlyError = errorString;
    
    // 将常见的错误消息转换为更友好的中文提示
    if (errorString.contains("Permission denied", Qt::CaseInsensitive) || 
        errorString.contains("Access is denied", Qt::CaseInsensitive)) {
        friendlyError = "权限不足，无法访问串口。\n请检查是否有其他程序正在使用该串口，或尝试以管理员身份运行。";
    } else if (errorString.contains("Device not found", Qt::CaseInsensitive) ||
               errorString.contains("No such device", Qt::CaseInsensitive)) {
        friendlyError = "设备未找到。\n请检查串口是否已正确连接，或尝试刷新串口列表。";
    } else if (errorString.contains("Resource busy", Qt::CaseInsensitive) ||
               errorString.contains("already in use", Qt::CaseInsensitive)) {
        friendlyError = "串口已被占用。\n请关闭其他正在使用该串口的程序后重试。";
    }
    
    QMessageBox::critical(this, "串口错误", friendlyError);
    updateConnectionStatus(false);
    systemPage->setPortEnabled(false);
    systemPage->setConnectionState(ConnectionState::Error);
    systemPage->appendLog(QString("错误: %1").arg(errorString), false);
}

void MainWindow::onSendCommandRequested(const QString &command)
{
    if (!serialPortManager->isPortOpen()) {
        QMessageBox::warning(this, "警告", "串口未连接，无法发送命令！");
        return;
    }
    
    if (serialPortManager->sendCommand(command)) {
        // 记录完整的发送数据（包括换行符等）
        QString logMessage = command;
        // 将不可见字符转换为可读格式
        logMessage.replace("\r", "\\r");
        logMessage.replace("\n", "\\n");
        systemPage->appendLog(logMessage, false);
    } else {
        QString errorMsg = "发送命令失败！\n\n可能的原因：\n- 串口连接已断开\n- 发送缓冲区已满\n- 设备无响应";
        QMessageBox::warning(this, "发送失败", errorMsg);
        systemPage->appendLog("错误: 发送命令失败", false);
    }
}

void MainWindow::onStartRequested()
{
    onSendCommandRequested("CMD:START\n");
}

void MainWindow::onStopRequested()
{
    onSendCommandRequested("CMD:STOP\n");
}

void MainWindow::onOpenPortRequested(const QString &portName, int baudRate)
{
    if (portName.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择串口！");
        systemPage->setConnectionState(ConnectionState::Error);
        return;
    }
    
    // 设置连接中状态
    systemPage->setConnectionState(ConnectionState::Connecting);
    systemPage->appendLog(QString("正在连接串口: %1 @ %2 baud...").arg(portName).arg(baudRate), false);
    
    if (!serialPortManager->openPort(portName, baudRate)) {
        QString errorMsg = QString("无法打开串口: %1\n\n可能的原因：\n- 串口已被其他程序占用\n- 串口不存在或已断开\n- 权限不足").arg(portName);
        QMessageBox::critical(this, "连接失败", errorMsg);
        systemPage->setPortEnabled(false);
        systemPage->setConnectionState(ConnectionState::Error);
    }
    // 如果成功，onPortOpened 会被调用，状态会更新为 Connected
}

void MainWindow::onClosePortRequested()
{
    serialPortManager->closePort();
}

void MainWindow::refreshPortList()
{
    QStringList ports = serialPortManager->getAvailablePorts();
    systemPage->setPortList(ports);
}

void MainWindow::updateFOCParameters()
{
    // 更新所有页面
    dashboardPage->updateData(focParams);
    motorAPage->updateData(focParams);
    motorBPage->updateData(focParams);
}

void MainWindow::updateConnectionStatus(bool connected)
{
    tuningPage->setControlsEnabled(connected);
}

void MainWindow::centerWindow()
{
    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        QRect windowGeometry = frameGeometry();
        windowGeometry.moveCenter(screenGeometry.center());
        move(windowGeometry.topLeft());
    }
}

void MainWindow::saveWindowState()
{
    QSettings settings;
    settings.setValue("window/size", size());
    settings.setValue("window/pos", pos());
    settings.setValue("window/maximized", isMaximized());
}

void MainWindow::restoreWindowState()
{
    QSettings settings;
    QSize savedSize = settings.value("window/size", QSize(800, 600)).toSize();
    QPoint savedPos = settings.value("window/pos", QPoint()).toPoint();
    bool maximized = settings.value("window/maximized", false).toBool();
    
    if (maximized) {
        showMaximized();
    } else {
        resize(savedSize);
        if (!savedPos.isNull()) {
            move(savedPos);
        } else {
            centerWindow();
        }
    }
}

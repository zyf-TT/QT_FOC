#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QTimer>
#include "models/FOCParameters.h"
#include "models/ControlMode.h"
#include "core/SerialPortManager.h"
#include "core/DataParser.h"
#include "widgets/TopBar.h"
#include "widgets/NavBar.h"
#include "pages/DashboardPage.h"
#include "pages/MotorAPage.h"
#include "pages/MotorBPage.h"
#include "pages/WaveformPage.h"
#include "pages/TuningPage.h"
#include "pages/SystemPage.h"
#include "styles/StyleSheet.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTabClicked(int index);
    void onDataReceived(const QByteArray &data);
    void onPortOpened(const QString &portName, int baudRate);
    void onPortClosed();
    void onPortError(const QString &errorString);
    void onSendCommandRequested(const QString &command);
    void onStartRequested();
    void onStopRequested();
    void onOpenPortRequested(const QString &portName, int baudRate);
    void onClosePortRequested();
    void refreshPortList();

private:
    // 核心功能
    SerialPortManager *serialPortManager;
    DataParser *dataParser;
    QTimer *refreshTimer;
    FOCParameters focParams;
    
    // UI组件
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    TopBar *topBar;
    NavBar *navBar;
    QStackedWidget *stackedWidget;
    
    // 页面
    DashboardPage *dashboardPage;
    MotorAPage *motorAPage;
    MotorBPage *motorBPage;
    WaveformPage *waveformPage;
    TuningPage *tuningPage;
    SystemPage *systemPage;
    
    void setupUI();
    void connectSignals();
    void updateFOCParameters();
    void updateConnectionStatus(bool connected);
    void centerWindow();
    void saveWindowState();
    void restoreWindowState();
};

#endif // MAINWINDOW_H
